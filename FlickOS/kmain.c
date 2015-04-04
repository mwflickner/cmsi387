#include "io.h"
//#include "gdt.h"
//#include "idt.h"
#include "screen.h"
#include "serial.h"
#include "pic.h"
#include "gdt_idt.h"
#include "printf.h"


void kmain (){
    gdt_init();

    init_printf((void*) 0,putc);

    char welcome[] = "Welcome to FlickOS! GDT Loaded. \n";
    unsigned int size = sizeof(welcome) - 1;
    fb_write(welcome, size);
    serial_write(welcome, size);
    char sweg[] = "Do you even sweg? \n";
    unsigned int swegSize = sizeof(sweg) - 1;
    fb_write(sweg, swegSize);
    printf("Address of idt_init() = %x \n", idt_init);
    idt_init();
    pic_init();
    asm volatile ("int $0x3");
    asm volatile ("int $0x4");
    asm volatile ("int $33");
}