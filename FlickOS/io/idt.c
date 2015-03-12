#include "idt.h"
#include "io.h"

unsigned int last_interrupt;

void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt){
    last_interrupt = interrupt;
    (void)cpu;
    (void)stack;
}





