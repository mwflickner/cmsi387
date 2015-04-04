#include "io.h"
//#include "gdt.h"
//#include "idt.h"
#include "screen.h"
#include "serial.h"
#include "pic.h"
#include "gdt_idt.h"
#include "printf.h"
#include "multiboot.h"



void kmain(unsigned int ebx){
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
    //asm volatile ("int $0x3");
    //asm volatile ("int $0x4");
    //asm volatile ("int $33");

    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    printf("mbinfo flags: %x \n", mbinfo->flags);
    if(mbinfo->mods_count != 1){
        char error[] = "error with mods_count \n";
        fb_write(error, sizeof(error)-1);
        return;
    }
    typedef void (*call_module_t)(void);
    /* ... */
    module_t *m = (module_t*) mbinfo->mods_addr;
    unsigned int address_of_module = m->mod_start;
    printf("address_of_module is %x \n", address_of_module);

    call_module_t start_program = (call_module_t) address_of_module;
    printf("about to start program \n");
    start_program();
    printf("program ran \n");
    /* we'll never get here, unless the module code returns */
}