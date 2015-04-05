#ifndef INCLUDE_GDT_IDT_H
#define INCLUDE_GDT_IDT_H

#include "idt.h"

void idt_init();
void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);
void gdt_init();


#endif /* INCLUDE_GDT_IDT_H */