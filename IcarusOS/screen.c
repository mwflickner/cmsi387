#include "io.h"

/* I/O Ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* I/O Port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/* Colors for FrameBuffer */
#define FB_BLACK     0
#define FB_BLUE      1
#define FB_GREEN     2
#define FB_CYAN      3
#define FB_RED       4
#define FB_MAGENTA   5
#define FB_BROWN     6
#define FB_LIGHT_GREY 7
#define FB_DARK_GREY 8
#define FB_LIGHT_BLUE 9
#define FB_LIGHT_GREEN 10
#define FB_LIGHT_CYAN 11
#define FB_LIGHT_RED  12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN 14
#define FB_WHITE     15




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
int cursorPosition = 0;
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg){
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}


/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos){
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

int fb_write(char *buf, unsigned int len){
    unsigned int i;
    //int size = sizeof(buf);
    for(i=0; i<len; i++){
        if('\n' == buf[i]){
            cursorPosition = (cursorPosition/80 + 1)*80;
            if(cursorPosition/80 > 25){
                cursorPosition = 0;
            }
            fb_move_cursor(cursorPosition);
            continue;
        }
        if('\b' == buf[i]){
            cursorPosition--;
            if(cursorPosition < 0){
                cursorPosition = 0;
            }
            fb_move_cursor(cursorPosition);
            fb_write_cell(2*cursorPosition, ' ', FB_BLACK, FB_GREEN);
            continue;
        }

        fb_write_cell(2*cursorPosition, buf[i], FB_BLACK, FB_GREEN);
        cursorPosition++;
    }
    fb_move_cursor(cursorPosition);
    return 0; 
}

int fb_clock_write(char *buf, unsigned int len){
    unsigned int i;
    for(i=0; i<len; i++){
        fb_write_cell(2*80 - 2*(len - i), buf[i], FB_BLACK, FB_GREEN);
    }
    return 0;
}

int clear_screen(){
    unsigned int i;
    for(i=0; i<2*80*25; i= i+2){
        fb_write_cell(i, ' ', FB_BLACK, FB_GREEN);
    }
    return 0;
}

void putc(void* p, char c){
    char temp[2];
    temp[0] = c;
    temp[1] = 0;
    fb_write(temp, 1);
    (void) p;
}
