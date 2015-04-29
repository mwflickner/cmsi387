#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void* memset (void* ptr, int value, uint32_t num);
void memcpy(uint8_t *dest, const char *src, uint32_t len);
int32_t strcmp(char *string1, char *string2);
char *strcpy(char *dest, const char *src);
uint32_t strlen(char *src);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);

#endif