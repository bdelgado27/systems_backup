/* =============================================================================================================================== */
/**
 * \file heap-alloc.c
 */
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* INCLUDES */

//#include <stdint.h>
#include "heapalloc.h"
#include "types.h"
#include "kernel-stub.h"
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* MACROS */

#define ADDR_SIZE (sizeof(address_t))
#define ALIGN_UP(addr) (addr % ADDR_SIZE == 0 ? addr : (addr + ADDR_SIZE) & (ADDR_SIZE - 1))

#define HEADER_TO_BLOCK(p) ((void*)((address_t)p + sizeof(header_s)))
#define BLOCK_TO_HEADER(p) ((header_s*)((address_t)p - sizeof(header_s)))
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* TYPES & STRUCTS */

typedef struct header {

  word_t         size;
  struct header* next;
  struct header* prev;
  
} header_s;
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/* STATICS */

/* A pair of sentinels to the free list, making the coding easier. */
header_s free_head = { .next = NULL, .prev = NULL, .size = 0 };
header_s free_tail = { .next = NULL, .prev = NULL, .size = 0 };

/* The current limit of the heap. */
static address_t heap_limit = (address_t)NULL;

/* The externally provided end of the statics, at which the heap will begin. */
//extern address_t statics_limit;
/* =============================================================================================================================== */



/* =============================================================================================================================== */
/**
 * Initialize the heap.  If it is already initialized, do nothing.
 */
void heap_init () {

  

  /* Continue only if the heap is uninitialized. */
  if (heap_limit != (address_t)NULL) return;

  /* Start the heap where the statics end. */
  heap_limit =  0x2000 + 0x3000;

  /* Initialize the sentinels that bookend the free block list. */
  free_head.next = &free_tail;
  free_tail.prev = &free_head;
  
} /* heap_init () */
/* =============================================================================================================================== */



/* =============================================================================================================================== */
void* allocate (int size) {

  /* Ensure that the heap is initialized. */
  heap_init();

  /* Blocks must always be allocated in word/address-sized chunks. */
  size = ALIGN_UP(size);
  
  /* Search the free list for a block of sufficient size. */
  header_s* current = free_head.next;
  while (current != &free_tail) {

    /* Is this block large enough? */
    if (current->size >= size) {

      /* Yes. Remove it from the list and use it. */
      current->prev->next = current->next;
      current->next->prev = current->prev;
      current->next       = NULL;
      current->prev       = NULL;
      break;

    } else {

      /* No. Move to the next block. */
      current = current->next;

    }
    
  }

  /* If we did not find a free block to use, make a new one. */
  if (current == &free_tail) {

    int block_size = sizeof(header_s) + size;
    current        = (header_s*)heap_limit;
    current->size  = size;
    heap_limit    += block_size;
    
  }

  return HEADER_TO_BLOCK(current);
  
} /* allocate() */
/* =============================================================================================================================== */



/* =============================================================================================================================== */
void deallocate (void* ptr) {

  /* Do nothing if there is no block. */
  if (ptr == NULL) return;

  /* Find the header. */
  header_s* header = BLOCK_TO_HEADER(ptr);
  
  /* Insert the block at the front of the free list. */
  header->next         = free_head.next;
  header->prev         = &free_head;
  free_head.next->prev = header;
  free_head.next       = header;
  
} /* deallocate () */
/* =============================================================================================================================== */
