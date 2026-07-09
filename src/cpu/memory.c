#include "memory.h"
#include "../lib/types.h"

u32 page_directory[1024] __attribute__((aligned(4096)));
u32 first_page_table[1024] __attribute__((aligned(4096)));

void init_paging() {
    for (unsigned int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;                 // kernel only, R/W enabled, page table is not present
        first_page_table[i] = (i * 0x1000) | 0x11;      // identity maps first 4Mib, kernel only, R/W enabled, page is present
    }

    page_directory[0] = ((u32) first_page_table) | 3;   // kernel only, R/W, present

    enable_paging(page_directory);
}