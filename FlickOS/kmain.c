#include "io.h"
//#include "gdt.h"
//#include "idt.h"
#include "screen.h"
#include "serial.h"
#include "pic.h"
#include "gdt_idt.h"
#include "printf.h"
#include "multiboot.h"
#include "breakpoint.h"
#include "kheap.h"
#include "paging.h"

uint32_t placement_address = 0; //(uint32_t)&end;

void kmain(unsigned int ebx){
    gdt_init();

    init_printf((void*) 0,putc);

    char welcome[] = "Welcome to SwegOS! GDT Loaded. \n";
    unsigned int size = sizeof(welcome) - 1;
    fb_write(welcome, size);
    serial_write(welcome, size);
    char sweg[] = "Do you even sweg? \n";
    unsigned int swegSize = sizeof(sweg) - 1;
    fb_write(sweg, swegSize);
    printf("Address of idt_init() = %x \n", idt_init);
    idt_init();
    pic_init();

    //this needs to come before paging stuff so multiboot can be including in paging
    multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
    printf("mbinfo flags: %x \n", mbinfo->flags);
    if(mbinfo->mods_count != 1){
        char error[] = "error with mods_count \n";
        fb_write(error, sizeof(error)-1);
        return;
    }
    typedef void (*call_module_t)(void);
    module_t *m = (module_t*) mbinfo->mods_addr;
    unsigned int address_of_module = m->mod_start;
    printf("address_of_module is %x \n", address_of_module);
    call_module_t start_program = (call_module_t) address_of_module + 1;//without this +1 everything fails
    /*now that the module is built we can use the mod_end as a placement address
        which allows for the module to be included in the paging */
    placement_address = m->mod_end;

    //Now we can start paging
    uint32_t a = kmalloc(8);
    initialize_paging();
    //uint32_t *ptr = (uint32_t*)0xA0000000;
    //uint32_t do_page_fault = *ptr;
    //printf("PageFault: %x", do_page_fault);
    
    //Malloc and Free heap testing
    uint32_t b = kmalloc(8);
    uint32_t c = kmalloc(8);
    printf("a: %x,", a);
    printf(", b: %x", b);
    printf("\nc: %x \n", c);

    kfree((void*)c);
    printf("C free");
    kfree((void*)b);
    uint32_t d = kmalloc(12);
    printf(", d: %x", d); 

    breakpoint();

    //now call start the program 
    printf("about to start program \n");
    start_program();
    printf("program ran \n");
    /* we'll never get here, unless the module code returns */
}