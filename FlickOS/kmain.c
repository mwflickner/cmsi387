#include "io.h"

/* I/O Ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* I/O Port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/* Frame Buffer Stuff */
#define FB_GREEN     2
#define FB_DARK_GREY 8
int sum_of_three (int arg1, int arg2, int arg3){
        return arg1 + arg2 + arg3;
}

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
*/
char *fb = (char *) 0x000B8000;
unsigned int cursorPosition = 0;
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}


/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

int write(char *buf, unsigned int len){
    unsigned int i=0;
    //int size = sizeof(buf);
    for(i=0; i<len; i++){
        fb_write_cell(2*cursorPosition, buf[i], FB_GREEN, FB_DARK_GREY);
        cursorPosition++;
    }
    fb_move_cursor(cursorPosition);
    return 0;
    
}

void kmain (){
    char welcome[] = "welcome! ";
    unsigned int size = sizeof(welcome) - 1;
    write(welcome, size);
    char sweg[] = "i cri everitiem :'(";
    unsigned int swegSize = sizeof(sweg) - 1;
    write(sweg, swegSize);

}