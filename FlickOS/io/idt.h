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

void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt);
void load_idt(void* p);


#endif /* INCLUDE_IDT_H */