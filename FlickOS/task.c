//code modified from http://www.jamesmolloy.co.uk/tutorial_html/9.-Multitasking.html

#include <stdint.h>
#include "task.h"
#include "paging.h"
#include "kheap.h"

// The currently running task.
volatile task_t *current_task;

// The start of the task linked list.
volatile task_t *ready_queue;

// Some externs are needed to access members in paging.c...
extern page_directory_t *kernel_directory;
extern page_directory_t *current_directory;
extern void alloc_frame(page_t*,int,int);
extern uint32_t initial_esp;
extern uint32_t read_eip();

// The next available process ID.
uint32_t next_pid = 1;


void move_stack(void *new_stack_start, uint32_t size){
    uint32_t i;
    // Allocate some space for the new stack.
    for( i = (uint32_t)new_stack_start; i >= ((uint32_t)new_stack_start-size); i -= 0x1000){
        // General-purpose stack is in user-mode.
        alloc_frame( get_page(i, 1, current_directory), 0 /* User mode */, 1 /* Is writable */ );
    } 

    // Flush the TLB by reading and writing the page directory address again.
    uint32_t pd_addr;
    asm volatile("mov %%cr3, %0" : "=r" (pd_addr));
    asm volatile("mov %0, %%cr3" : : "r" (pd_addr)); 

     // Old ESP and EBP, read from registers.
    uint32_t old_stack_pointer;
    asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
    uint32_t old_base_pointer;
    asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));

    uint32_t offset = (uint32_t)new_stack_start - initial_esp;

    uint32_t new_stack_pointer = old_stack_pointer + offset;
    uint32_t new_base_pointer  = old_base_pointer  + offset; 

    // Copy the stack.
    memcpy((void*)new_stack_pointer, (void*)old_stack_pointer, initial_esp - old_stack_pointer);

    // Backtrace through the original stack, copying new values into
    // the new stack.
    for(i = (uint32_t)new_stack_start; i > (uint32_t)new_stack_start-size; i -= 4){
       uint32_t tmp = * (uint32_t*)i;
       // If the value of tmp is inside the range of the old stack, assume it is a base pointer
       // and remap it. This will unfortunately remap ANY value in this range, whether they are
       // base pointers or not.
       if (( old_stack_pointer < tmp) && (tmp < initial_esp)){
         tmp = tmp + offset;
         uint32_t *tmp2 = (uint32_t*)i;
         *tmp2 = tmp;
       }
    } 

    // Change stacks.
    asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
    asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer));

}


void initialise_tasking(){

    asm volatile("cli");

    // Relocate the stack so we know where it is.
    move_stack((void*)0xE0000000, 0x2000);

    // Initialise the first task (kernel task)
    current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
    current_task->id = next_pid++;
    current_task->esp = current_task->ebp = 0;
    current_task->eip = 0;
    current_task->page_directory = current_directory;
    current_task->next = 0;

    // Reenable interrupts.
    asm volatile("sti");
} 


int fork(){
    // We are modifying kernel structures, and so cannot be interrupted.
    asm volatile("cli");

    // Take a pointer to this process' task struct for later reference.
    task_t *parent_task = (task_t*)current_task;

    // Clone the address space.
    page_directory_t *directory = clone_directory(current_directory);

       // Create a new process.
    task_t *new_task = (task_t*)kmalloc(sizeof(task_t));
    new_task->id = next_pid++;
    new_task->esp = new_task->ebp = 0;
    new_task->eip = 0;
    new_task->page_directory = directory;
    new_task->next = 0;

    // Add it to the end of the ready queue.
    // Find the end of the ready queue...
    task_t *tmp_task = (task_t*)ready_queue;
    while (tmp_task->next)
        tmp_task = tmp_task->next;
    // ...And extend it.
    tmp_task->next = new_task;

    // This will be the entry point for the new process.
    uint32_t eip = read_eip(); 

    // We could be the parent or the child here - check.
    if (current_task == parent_task){
        // We are the parent, so set up the esp/ebp/eip for our child.
        uint32_t esp; asm volatile("mov %%esp, %0" : "=r"(esp));
        uint32_t ebp; asm volatile("mov %%ebp, %0" : "=r"(ebp));
        new_task->esp = esp;
        new_task->ebp = ebp;
        new_task->eip = eip;
        // All finished: Reenable interrupts.
        asm volatile("sti"); 
        return new_task->id;
    }
    else {
        // We are the child - by convention return 0.
        return 0;
    }
} 