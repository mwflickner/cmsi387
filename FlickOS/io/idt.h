#ifndef INCLUDE_IDT_H
#define INCLUDE_IDT_H

struct cpu_state {
    register int eax nasm("eax");
    register int ebx nasm("ebx");
    register int ecx nasm("ecx");
    register int edx nasm("edx");
    register int esp nasm("esp");
    register int ebp nasm("ebp");
    register int esi nasm("esi");
    register int edi nasm("edi");
} __attribute__((packed));

struct stack_state {
    unsigned int error_code = [esp];
    unsigned int eip = [esp+4];
    unsigned int cs = [esp+8];
    unsigned int eflags = [esp+12];
} __attribute__((packed));

void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt)

#endif /* INCLUDE_IDT_H */