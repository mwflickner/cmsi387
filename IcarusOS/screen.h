#ifndef INCLUDE_SCREEN_H
#define INCLUDE_SCREEN_H

int fb_write(char *buf, unsigned int len);

int fb_clock_write(char *buf, unsigned int len);

int clear_screen();

void putc ( void* p, char c);

#endif /* INCLUDE_SCREEN_H */