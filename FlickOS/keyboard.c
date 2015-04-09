#include "io.h"
#include "printf.h"
#include "screen.h"

#include <stdint.h>

#define KBD_DATA_PORT   0x60

/** read_scan_code:
 *  Reads a scan code from the keyboard
 *
 *  @return The scan code (NOT an ASCII character!)
 */
unsigned char read_scan_code(void){
    unsigned char scanCode = inb(KBD_DATA_PORT);
    //printf("ScanCode = %x \n", (int)scanCode);
    return scanCode;
}


static unsigned char kbdus1[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,        /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
}; 

static unsigned char kbdus2[128] =
{
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
    '(', ')', '_', '+', '\b', /* Backspace */
    0,         /* Shift-Tab is zero for now */
    'Q', 'W', 'E', 'R',   /* 19 */
    'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0,          /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
    '\"', '~',   0,        /* Left shift */
    '\\', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
    'M', '<', '>', '?',   0,              /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
    '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
    '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};

enum {CapsRelease = 0xBA, leftShift = 0x2A, rightShift = 0x36, leftShiftRelease = 0xAA, rightShiftRelease= 0xB6};
static uint8_t CapLocks = 0;
static uint8_t ShiftL = 0;
static uint8_t ShiftR = 0;
int magicNum = 'a' - 'A';

uint8_t toUpperCase(uint8_t character){
   return character - magicNum;
}

uint8_t toLowerCase(uint8_t character){
    return character + magicNum;
}

uint8_t convertToAscii(const uint8_t code){

    if(code == CapsRelease){
        CapLocks = !CapLocks;
        //printf("capslocks = %d ", CapLocks);
        return 0;
    }
    if(code == leftShift){
        ShiftL = 1;
        return 0;
    }
    if(code == leftShiftRelease){
        ShiftL = 0;
        return 0;
    }
    if(code == rightShift){
        ShiftR = 1;
        return 0;
    }
    if(code == rightShiftRelease){
        ShiftR = 0;
        return 0;
    }

    if(code > 0x7F){
        return 0;
    }

    uint8_t temp;
    if(ShiftL || ShiftR){
        temp = kbdus2[code & 0x7F];
        return temp;
    }
    else {
        temp = kbdus1[code & 0x7F];
    }

    if(CapLocks && (ShiftL || ShiftR)){
        return temp;
    }
    if(CapLocks || ShiftL || ShiftR){
        if(temp == 0){
            return temp;
        }
        else {
            return toUpperCase(temp);
        } 
    }

    return temp;
}

void echo_keyboard(){
    char temp[2];
    temp[0] = convertToAscii(read_scan_code());
    if(temp[0] == 0){
        return;
    }

    temp[1] = 0;
    fb_write(temp, 1);
}