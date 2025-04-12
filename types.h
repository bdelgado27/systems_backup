#if !defined (_TYPES_H)
#define _TYPES_H

#include <stdint.h>

#define NULL 0

typedef uint32_t address_t;
typedef uint32_t word_t;

typedef struct dt_entry {
  word_t    type;
  address_t base;
  address_t limit;
} dt_entry_s;

typedef struct DMA_portal {
  address_t src;
  address_t dst;
  word_t    length;
} DMA_portal_s;

#endif /* _TYPES_H */
