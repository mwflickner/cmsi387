#include <stdint.h>
#include "gdt_idt.h"
#include "paging.h"
#include "printf.h"
#include "util.h"
#include "kheap.h"
#include "breakpoint.h"
//code from http://www.jamesmolloy.co.uk/tutorial_html/6.-Paging.html
// The kernel's page directory
page_directory_t *kernel_directory=0;

// The current page directory;
page_directory_t *current_directory=0;

// A bitset of frames - used or free.
uint32_t *frames;
uint32_t nframes;

// Defined in kheap.c
extern uint32_t placement_address;

// Macros used in the bitset algorithms.
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset
static void set_frame(uint32_t frame_addr) {
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

// Static function to clear a bit in the frames bitset
static void clear_frame(uint32_t frame_addr) {
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}
#if 0
// Static function to test if a bit is set.
static uint32_t test_frame(uint32_t frame_addr) {
   uint32_t frame = frame_addr/0x1000;
   uint32_t idx = INDEX_FROM_BIT(frame);
   uint32_t off = OFFSET_FROM_BIT(frame);
   return (frames[idx] & (0x1 << off));
}
#endif
// Static function to find the first free frame.
static uint32_t first_frame() {
   uint32_t i, j;
   for (i = 0; i < INDEX_FROM_BIT(nframes); i++) {
       // nothing free, exit early.
       if (frames[i] != 0xFFFFFFFF) {
           // at least one bit is free here.
           for (j = 0; j < 32; j++) {
               uint32_t toTest = 0x1 << j;
               if (!(frames[i]&toTest)){
                   return i*4*8+j;
               }
           }
       }
   }
   return 0;
}



// Function to allocate a frame.
void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
   if (page->frame != 0) {
       return; // Frame was already allocated, return straight away.
   }
   else {
       uint32_t idx = first_frame(); // idx is now the index of the first free frame.
       if (idx == (uint32_t)-1) {
           // PANIC is just a macro that prints a message to the screen then hits an infinite loop.
           PANIC("No free frames!");
       }
       set_frame(idx*0x1000); // this frame is now ours!
       page->present = 1; // Mark it as present.
       page->rw = (is_writeable)?1:0; // Should the page be writeable?
       page->user = (is_kernel)?0:1; // Should the page be user-mode?
       page->frame = idx;
   }
}

// Function to deallocate a frame.
void free_frame(page_t *page) {
   uint32_t frame;
   if (!(frame=page->frame)) {
       return; // The given page didn't actually have an allocated frame!
   }
   else {
       clear_frame(frame); // Frame is now free again.
       page->frame = 0x0; // Page now doesn't have a frame.
   }
}

void switch_page_directory(page_directory_t *dir) {
   current_directory = dir;
   asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   breakpoint();
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(uint32_t address, int make, page_directory_t *dir) {
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   uint32_t table_idx = address / 1024;
   // If this table is already assigned
   if (dir->tables[table_idx]) {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make) {
       uint32_t tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else {
       return 0;
   }
}

void page_fault(uint32_t err_code) {
    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // The error code gives us details of what happened.
    int present   = !(err_code & 0x1); // Page not present
    int rw = err_code & 0x2;           // Write operation?
    int us = err_code & 0x4;           // Processor was in user-mode?
    int reserved = err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    //int id = err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.
    printf("Page fault! ( ");
    if(present){
        printf("present ");
    }
    if(rw){
        printf("read-only ");
    }
    if(us){
        printf("user-mode ");
    }
    if(reserved){
        printf("reserved ");
    }
    printf(") at 0x %x \n", faulting_address);
    PANIC("panic");
}

void initialize_paging() {
   // The size of physical memory. For the moment we
   // assume it is 16MB big.
   uint32_t mem_end_page = 0x1000000;

   nframes = mem_end_page / 0x1000;
   frames = (uint32_t*)kmalloc(INDEX_FROM_BIT(nframes));
   memset(frames, 0, INDEX_FROM_BIT(nframes));

   // Let's make a page directory.
   kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
   memset(kernel_directory, 0, sizeof(page_directory_t));
   current_directory = kernel_directory;

   // We need to identity map (phys addr = virt addr) from
   // 0x0 to the end of used memory, so we can access this
   // transparently, as if paging wasn't enabled.
   // NOTE that we use a while loop here deliberately.
   // inside the loop body we actually change placement_address
   // by calling kmalloc(). A while loop causes this to be
   // computed on-the-fly rather than once at the start.
   uint32_t i = 0;
   while (i < placement_address) {
       // Kernel code is readable but not writeable from userspace.
       alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
       i += 0x1000;
   }
   // Before we enable paging, we must register our page fault handler.
   //register_interrupt_handler(14, page_fault);

   // Now, enable paging!
   switch_page_directory(kernel_directory);
}