#include "pic.h"
#include "io.h"
#include "idt.h"
#include "screen.h"
#include "gdt.h"

//GDT Code
enum {gdtSize = 5};

gdt_ptr gdtP;
gdt_entry gdtTable[gdtSize];

// Set the value of one GDT entry.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
   gdtTable[num].base_low    = (base & 0xFFFF);
   gdtTable[num].base_middle = (base >> 16) & 0xFF;
   gdtTable[num].base_high   = (base >> 24) & 0xFF;

   gdtTable[num].limit_low   = (limit & 0xFFFF);
   gdtTable[num].granularity = (limit >> 16) & 0x0F;

   gdtTable[num].granularity |= gran & 0xF0;
   gdtTable[num].access      = access;
} 


void gdt_init() {
   gdtP.limit = (sizeof(gdtTable) - 1);
   gdtP.base = &gdtTable[0];

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   gdt_flush(&gdtP);
}




//IDT CODE


idt_ptr_t idtP;

idt_entry idtTable[48];

//macro to fill table
#define FILL_IDT(a)\
    extern unsigned int interrupt_handler_##a;\
    idt[a].low = interrupt_handler_##a & 0x0000FFFF;\
    idt[a].segmentSelector = 0x08;\
    idt[a].always0 = 0x0;\
    idt[a].flags = 0x8E;\
    idt[a].high = interrupt_handler_##a >> 16;

void fill_idt_table(idt_entry* idt){
    FILL_IDT(0);
    FILL_IDT(1);
    FILL_IDT(2);
    FILL_IDT(3);
    FILL_IDT(4);
    FILL_IDT(5);
    FILL_IDT(6);
    FILL_IDT(7);
    FILL_IDT(8);
    FILL_IDT(9);
    FILL_IDT(10);
    FILL_IDT(11);
    FILL_IDT(12);
    FILL_IDT(13);
    FILL_IDT(14);
    FILL_IDT(15);
    FILL_IDT(16);
    FILL_IDT(17);
    FILL_IDT(18);
    FILL_IDT(19);
    FILL_IDT(20);
    FILL_IDT(21);
    FILL_IDT(22);
    FILL_IDT(23);
    FILL_IDT(24);
    FILL_IDT(25);
    FILL_IDT(26);
    FILL_IDT(27);
    FILL_IDT(28);
    FILL_IDT(29);
    FILL_IDT(30);
    FILL_IDT(31);
}

void idt_init(){
    fill_idt_table(idtTable);
    idtP.limit = sizeof(idtTable);
    idtP.base = &idtTable[0];
    load_idt(&idtP);
}


void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt){
    unsigned int last_interrupt = interrupt;
    fb_write("X", 1);
    (void)cpu;
    (void)stack;
    
    //must be last statement
    pic_acknowledge(last_interrupt);
}

