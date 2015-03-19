#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

struct gdt {
    unsigned int address;
    unsigned short size;
} __attribute__((packed));

void setGdt(unsigned long* globalDescriptorTable, unsigned int sizeOfGDT);

#endif /* INCLUDE_GDT_H */