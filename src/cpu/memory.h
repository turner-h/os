#ifndef MEMORY_H
#define MEMORY_H

#include "../lib/types.h"

// one million pages!!!!
#define TOTAL_PAGES 1048576

extern void enable_paging(unsigned int *page_directory);
extern u32 kernel_end;

void init_paging();

#endif