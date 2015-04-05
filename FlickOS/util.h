#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void* memset (void* ptr, int value, uint32_t num);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
extern void panic(const char *message, const char *file, uint32_t line);

#endif