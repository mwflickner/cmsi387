#include "io.h"
#include "pic.h"
#include "printf.h"


#define PIC1_PORT_A 0x20
#define PIC2_PORT_A 0xA0

/* The PIC interrupts have been remapped */
#define PIC1_START_INTERRUPT 0x20
#define PIC2_START_INTERRUPT 0x28
#define PIC2_END_INTERRUPT   PIC2_START_INTERRUPT + 7

#define PIC_ACK     0x20

void pic_init(){
    //Initalization
    /* ICW1 */
    outb( 0x20, 0x11 ); /* Master port A */
    outb( 0xA0, 0x11 ); /* Slave port A */

    /* ICW2 */
    outb( 0x21, 0x20 ); /* Master offset of 0x20 in the IDT */
    outb( 0xA1, 0x28 ); /* Master offset of 0x28 in the IDT */

    /* ICW3 */
    outb( 0x21, 0x04 ); /* Slaves attached to IR line 2 */
    outb( 0xA1, 0x02 ); /* This slave in IR line 2 of master */

    /* ICW4 */
    outb( 0x21, 0x05 ); /* Set as master */
    outb( 0xA1, 0x01 ); /* Set as slave */

    //Set the interrupt mask
    /* Only listen to irqs 0, 1, and 2 */
    outb( 0x21, 0xfd ); /* master PIC */
    outb( 0xA1, 0xff ); /* slave PIC */
    asm volatile("sti");
}
/** pic_acknowledge:
 *  Acknowledges an interrupt from either PIC 1 or PIC 2.
 *
 *  @param num The number of the interrupt
 */
void pic_acknowledge(unsigned int interrupt){
    if (interrupt < PIC1_START_INTERRUPT || interrupt > PIC2_END_INTERRUPT) {
        return;
    }

    if (interrupt < PIC2_START_INTERRUPT) {
        outb(PIC1_PORT_A, PIC_ACK);
    } else {
        outb(PIC2_PORT_A, PIC_ACK);
    }
}

