#include "util.h"
#include "printf.h"

void* memset (void* ptr, int value, uint32_t num){
	uint8_t *array = ptr;
	uint8_t val = (uint8_t)value;
	uint32_t i;
	for(i=0; i<num; i++){
		array[i] = val;
	}
	return array;
}

extern void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    printf("PANIC(%s) at %s: %s \n", message, file, line);
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    printf("ASSERTION-FAILED(%s) at %s: %s \n", desc, file, line);
    // Halt by going into an infinite loop.
    for(;;);
}