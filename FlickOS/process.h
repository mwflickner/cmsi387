#ifndef INCLUDE_PROCESS_H
#define INCLUDE_PROCESS_H

#include <stdint.h>

uint32_t read_eip();

void copy_page_physical(uint32_t src_frame, uint32_t table_frame);

#endif