#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

struct cpu_state {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
    unsigned int esp;
    unsigned int ebp;
    unsigned int esi;
    unsigned int edi;
} __attribute__((packed));

struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

typedef struct {
    unsigned short low;
    unsigned short segmentSelector;
    unsigned char always0;
    unsigned char flags;
    unsigned short high;
} __attribute__((packed)) idt_entry;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   unsigned short limit;
   idt_entry *base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;


void load_idt(idt_ptr_t *p);


#endif /* INCLUDE_IDT_H */