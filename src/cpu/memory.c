#include "memory.h"
#include "../lib/types.h"
#include "../lib/util.h"

u32 *g_page_bitmap;
u32 *g_first_page_addr;

u32* pframe_alloc() {
    int i = 0;
    while (g_page_bitmap[i] == 0xFFFFFFFF) {
        i++;
    }

    int j = 0;
    u32 index = 1;
    while (g_page_bitmap[i] & index == 0) {
        j++;
        index << 1;
    }

    return g_first_page_addr + ((i * 32) << 12) + (j << 12); 
}

void pframe_free(u32* addr) {
    int offset = g_first_page_addr - addr;
    int offset_pages = offset >> 12;
    g_page_bitmap[offset_pages / 32] ^= (1 << offset % 32);
}

void init_paging() {
    g_page_bitmap = (u32 *) ((((u32) &kernel_end >> 12) + 1) << 12);  // gets next frame after kernel
    g_first_page_addr = g_page_bitmap;

    *g_page_bitmap = 0xFFFFFFFF;
    for (int i = 1; i < (TOTAL_PAGES / 32) - 1; i++) {
        *(g_page_bitmap + i) = 0;
    }

    u32* page_directory = pframe_alloc();
    u32* first_page_table = pframe_alloc();

    for (unsigned int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;                 // kernel only, R/W enabled, page table is not present
        first_page_table[i] = (i * 0x1000) | 0x11;      // identity maps first 4Mib, kernel only, R/W enabled, page is present
    }

    page_directory[0] = ((u32) first_page_table) | 3;   // kernel only, R/W, present

    enable_paging(page_directory);
}

