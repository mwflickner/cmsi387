#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

#include <stdint.h>

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
typedef struct {
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) gdt_entry;

typedef struct {
    uint16_t limit;
    gdt_entry *base;
} __attribute__((packed)) gdt_ptr;

void gdt_flush(gdt_ptr *p);

#endif /* INCLUDE_GDT_H */