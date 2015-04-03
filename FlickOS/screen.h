#ifndef INCLUDE_SCREEN_H
#define INCLUDE_SCREEN_H

int fb_write(char *buf, unsigned int len);

int serial_write(char *buf, unsigned int len);

void putc ( void* p, char c);

#endif /* INCLUDE_SCREEN_H */