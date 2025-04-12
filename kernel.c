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

    }


    INSERT(tail, block);
  }

  return;
}
void run_new_program(word_t next_program_ROM){

  //unusable currently but meant to check if there is one program or not in order to turn the alarm off 

  // if(program_count <1){
  //  alarm_off();}
  // if(program_count >= 1){
  //   alarm_setup;}
	  

  //check if its first program (our init program will always run rom 4)
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
  curr = block;
	
  program_count += 1;

  // if(next_program_ROM == 4){
  //   alarm_setup();
  // }
  userspace_jump(limit);
}

void alarm_int(int pc){
    curr->pc = pc;
    curr->sp = get_sp();
    curr->fp = get_fp();
    int base = curr->next->base;
    int next_pc = curr->next->pc;
    int addr = base+next_pc;
    curr = curr->next;
    char stry[32];
    int_to_hex(addr, stry);
    print(stry);
    print("\n");
    processspace_jump(addr);
}

void exit_process(){
  //if last process then return and end
  if(process_head->prev == process_head){
    return;
  }

  //else readd newly avail address to list 
  int curr_addr = curr->base;
  int_link* block = allocate(sizeof(int_link));
  if (block == NULL) {
    return;
  }
  block->addr = curr_addr;
  INSERT(tail, block);

  //and remove node from process circle
  REMOVE(curr->next, curr);
 
  

  int next_base = curr->next->base;
  int next_pc = curr->next->pc;
  int next_addr = next_base+next_pc;
  process_link* temp = curr; 
  curr = curr->next;
  deallocate(temp);
  program_count -= 1;
  userspace_jump(next_addr);
}

