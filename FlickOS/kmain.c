#include "io.h"
//#include "gdt.h"
//#include "idt.h"
#include "screen.h"
#include "pic.h"
#include "gdt_idt.h"


void kmain (){
    //gdt_load();
    char welcome[] = "Welcome to FlickOS! GDT Loaded. ";
    unsigned int size = sizeof(welcome) - 1;
    fb_write(welcome, size);
    serial_write(welcome, size);
    char sweg[] = "Do you even sweg?";
    unsigned int swegSize = sizeof(sweg) - 1;
    fb_write(sweg, swegSize);
    //fill_idt_table(idtTable);
    //load_idt(idtTable);
    idt_init();
    //pic_init();
}