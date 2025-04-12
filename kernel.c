/* =============================================================================================================================== */
/* INCLUDES */

#include "kernel-stub.h"
#include "types.h"
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

  int limit = 0x10000 + 0x3000;
  // if(head == NULL){
  //   //wrong
  //   return;
  // }  
  // else{
  //   //modify
  //   limit = head->addr;
  //   REMOVE(head->next, head);
  // }

  /* Copy the program into the free RAM space after the kernel. */
  DMA_portal_ptr->src    = dt_ROM_ptr->base;
  DMA_portal_ptr->dst    = limit;
  DMA_portal_ptr->length = dt_ROM_ptr->limit - dt_ROM_ptr->base; // Trigger

  /* Jump to the copied code at the kernel limit / program base. */
  // process_link* block = malloc(sizeof(*block));
  // block->base = dt_ROM_ptr->base;
  // block->limit = limit;
  // block->fp = limit;
  // block->sp = limit;
  // block->pc = 0;
  // if(process_head == NULL){
  //   process_head = block;
  //   process_tail = block;
  // }
  // else{
  //   INSERT(process_head, block);
  //   process_tail->prev = block;
  //   head->prev = process_tail;
  // }
  // curr = block;
	
  //program_count += 1;
  userspace_jump(limit);
}
