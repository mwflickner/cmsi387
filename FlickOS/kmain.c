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

    uint32_t a = kmalloc(8);
    initialize_paging();
    //uint32_t *ptr = (uint32_t*)0xA0000000;
    //uint32_t do_page_fault = *ptr;
    //printf("PageFault: %x", do_page_fault);
    

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

    call_module_t start_program = (call_module_t) address_of_module + 1;  //without this +1 everything fails
    printf("about to start program \n");
    breakpoint();
    start_program();
    printf("program ran \n");
    /* we'll never get here, unless the module code returns */
}