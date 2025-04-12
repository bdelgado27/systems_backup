/* =============================================================================================================================== */
/* INCLUDES */

#include "kernel-stub.h"
#include "types.h"
#include "linked_list.h"
#include "heapalloc.h"
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* CONSTANTS */

#define BYTES_PER_WORD  4
#define BITS_PER_BYTE   8
#define BITS_PER_NYBBLE 4

static char hex_digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
/* =============================================================================================================================== */



/* =============================================================================================================================== */
void int_to_hex (word_t value, char* buffer) {

  /* Traverse the value from most to least significant bits. */
  for (int shift = BYTES_PER_WORD * BITS_PER_BYTE - BITS_PER_NYBBLE; shift >= 0; shift -= BITS_PER_NYBBLE) {

    /* Grab the next nybble and add its corresponding digit to the string, advancing the string's pointer. */
    word_t nybble = (value >> shift) & 0xf;
    *buffer++ = hex_digits[nybble];
    
  }

  /* Finish the string with a null termination. */
  *buffer = '\0';
  
} /* int_to_hex () */
/* =============================================================================================================================== */



/* =============================================================================================================================== */
void run_programs () {

  static word_t next_program_ROM = 3;
  
  /* Find the next program ROM in the device table. */
  char str_buffer[9];
  print("Searching for ROM #");
  int_to_hex(next_program_ROM, str_buffer);
  print(str_buffer);
  print("\n");
  dt_entry_s* dt_ROM_ptr = find_device(ROM_device_code, next_program_ROM++);
  if (dt_ROM_ptr == NULL) {
    return;
  }

  print("Running program...\n");

  /* Copy the program into the free RAM space after the kernel. */
  DMA_portal_ptr->src    = dt_ROM_ptr->base;
  DMA_portal_ptr->dst    = kernel_limit;
  DMA_portal_ptr->length = dt_ROM_ptr->limit - dt_ROM_ptr->base; // Trigger

  /* Jump to the copied code at the kernel limit / program base. */
  userspace_jump(kernel_limit);
  
} /* run_programs () */
/* =============================================================================================================================== */


// RAM Blocks linked list nodes 
typedef struct int_link {
  struct int_link* next;
  struct int_link* prev;
  int              addr;
} int_link;
int_link* tail = NULL;
int_link* head = NULL;


// Process info circular linked list nodes
typedef struct process_link {
  struct process_link* next;
  struct process_link* prev;
  int base;
  int limit;
  int pc;
  int sp;
  int fp;
} process_link;
process_link* process_head = NULL;
process_link* process_tail = NULL;
process_link* curr = NULL;
int program_count = 0;



void divide_ram(){
  int ram_size = 1024 * 128;
  int block_size = 32 * 1024;
  int num_blocks = ram_size/block_size;
  
  for(int i = 2; i<num_blocks; i++){
    int_link* block = allocate(sizeof(int_link));
    if (block == NULL) {
      return;
    }
    block->addr = i*block_size;
    if(i == 2){
      head = block;
      // char strh[32];
      // int_to_hex(head->addr, strh);
      // print(strh);
      // print("\n");
    }
    //  char strx[32];
    //  int_to_hex(block->addr, strx);
    //  print(strx);
    //  print("\n");

    INSERT(tail, block);
  }
//   int y = head->addr;
//    int x = head->prev->addr;

//  char strx[32];
//  int_to_hex(x, strx);
//  print(strx);

//  print("\n");

//  char stry[32];
//  int_to_hex(y, stry);
//  print(stry);
//  print("\n");
  return;
}
void run_new_program(word_t next_program_ROM){

  // if(program_count <1){
  //  alarm_off();}
  // if(program_count >= 1){
  //   alarm_setup;}
	  
  
  /* Find the next program ROM in the device table. */
  char str_buffer[9];
  print("Searching for ROM #");
  int_to_hex(next_program_ROM, str_buffer);
  print(str_buffer);
  print("\n");
  dt_entry_s* dt_ROM_ptr = find_device(ROM_device_code, next_program_ROM);
  if (dt_ROM_ptr == NULL) {
    return;
  }

  print("Running program...\n");

  int limit = 0;
  if(head == NULL){
    //wrong
    return;
  }  
  else{
    //modify
    limit = head->addr + 0x3000;
    int_link* temp = head->prev;
    REMOVE(head->prev, head);
    deallocate(head);
    head = temp;
  }

  /* Copy the program into the free RAM space after the kernel. */
  DMA_portal_ptr->src    = dt_ROM_ptr->base;
  DMA_portal_ptr->dst    = limit;
  DMA_portal_ptr->length = dt_ROM_ptr->limit - dt_ROM_ptr->base; // Trigger

  /* Jump to the copied code at the kernel limit / program base. */


  process_link* block = allocate(sizeof(process_link));
  block->base = limit;
  block->limit = limit + dt_ROM_ptr->limit - dt_ROM_ptr->base;
  block->fp = limit + dt_ROM_ptr->limit - dt_ROM_ptr->base;
  block->sp = limit + dt_ROM_ptr->limit - dt_ROM_ptr->base;
  block->pc = 0;

  CIRCULAR_INSERT(process_head, process_tail, block);
  // if(process_head == NULL){
  //   process_head = block;
  //   process_tail = block;
  // }
  // else{
  //   CIRCULAR_INSERT(process_tail, block);
  //   process_head->prev = block;
  //   process_tail->prev = process_head;
  // }
  curr = block;
	
  program_count += 1;

  userspace_jump(limit);
}
