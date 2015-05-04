#include <stdint.h>
#include "io.h"
#include "gdt_idt.h"
#include "screen.h"
#include "printf.h"
#include "task.h"


uint32_t ticks = 0;

void timer_callback(){
    ticks++;
    //printf("timer");
    //switch_task();
    if(ticks % 32 == 0){
        char timer[32];
        sprintf(timer, "%d", ticks);
        fb_clock_write(timer, sizeof(timer));
   }
}

void timer_init(uint32_t frequency){

    uint32_t divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
    printf("timer initiated");
}