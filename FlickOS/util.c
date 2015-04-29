#include <stdint.h>
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

void memcpy(uint8_t *dest, const char *src, uint32_t len){
    const uint8_t *sp = (const uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

//returns -1 if String1 < String2
//returns 1 if String1 > String2 or if they are the same length
//and String1 != String2
//returns 0 if String1 = String2
int32_t strcmp(char *string1, char *string2){
    uint32_t i;
    int32_t result;
    //returns -1 if string1 is smaller than string2
    if(sizeof(string1) < sizeof(string2)){
        result = -1;
        return result;
    }
    //returns 1 if string2 is smaller than string1
    if(sizeof(string1) > sizeof(string2)){
        result = 1;
        return result;
    }
    //string1 and string2 are the same size
    for(i=0; i<sizeof(string1); i++){
        if(string1[i] != string2[i]){
            result = 1;
            return result;
        }
    }
    result = 0;
    return result;
}

char *strcpy(char *dest, const char *src){
    do {
      *dest++ = *src++;
    }
    while (*src != 0);
    //had to add return to get compiling
    return dest;
}


uint32_t strlen(char *src){
    uint32_t i = 0;
    while (*src++)
        i++;
    return i;
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