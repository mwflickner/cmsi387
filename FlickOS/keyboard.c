#include "io.h"
#include "printf.h"

#define KBD_DATA_PORT   0x60

/** read_scan_code:
 *  Reads a scan code from the keyboard
 *
 *  @return The scan code (NOT an ASCII character!)
 */
unsigned char read_scan_code(void){
    unsigned char scanCode = inb(KBD_DATA_PORT);
    printf("ScanCode = %x \n", (int)scanCode);
    return scanCode;
}

