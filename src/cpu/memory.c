#include "memory.h"
#include "../lib/types.h"
#include "../lib/util.h"
#include "../drivers/vga.h"

u32* g_page_bitmap;
u32* g_first_page_addr;

HeapData g_heap_data;

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

void init_pframe_allocator() {
    g_page_bitmap = (u32 *) ((((u32) &kernel_end >> 12) + 1) << 12);  // gets next frame after kernel
    g_first_page_addr = g_page_bitmap;

    for (int i = 0; i < 32; i++) {
        map_page(pframe_alloc(), (u8*) g_page_bitmap + (i * 0x1000), 3);
    }

    *g_page_bitmap = 0xFFFFFFFF;
    for (int i = 1; i < (TOTAL_PAGES / 32) - 1; i++) {
        *(g_page_bitmap + i) = 0;
    }
}

void* get_physical_addr(void* virt_addr) {
    u32 pd_index = (u32) virt_addr >> 22;
    u32 pt_index = ((u32) virt_addr >> 12) & 0x03FF;
    
    u32* page_directory = (u32*) PAGE_DIRECTORY_VADDR;
    u32* page_table = (u32*) (page_directory[pd_index] & 0xFFFFF000);

    return (void *) ((page_table[pt_index] & 0xFFFFF000) + ((u32) virt_addr & 0x0FFF));
}

int map_page(void* phys_addr, void* virt_addr, int flags) {
    u32 pd_index = (u32) virt_addr >> 22;
    u32 pt_index = ((u32) virt_addr >> 12) & 0x03FF;
    
    u32* page_directory = (u32*) PAGE_DIRECTORY_VADDR;
    if (!(page_directory[pd_index] & 0x1)) {   // pt doesnt exist
        u32* page_table = pframe_alloc();
        page_directory[pd_index] = (u32) page_table | (flags) | 0x1;

        for (int i = 0; i < 1024; i++) {
            page_table[i] = 0;
        }
    } 

    u32* page_table = (u32*) (0xFFC00000 + (pd_index * 0x1000));
    if (page_table[pt_index] & 0x1) {
        return 1;
    }

    page_table[pt_index] = ((u32) phys_addr & 0xFFFFF000) | (flags & 0xFFF) | 0x1;
    return 0;
}

void unmap_page(void* virt_addr) {
    u32 pd_index = (u32) virt_addr >> 22;
    u32 pt_index = ((u32) virt_addr >> 12) & 0x03FF;
    
    u32* page_directory = (u32*) PAGE_DIRECTORY_VADDR;
    u32* page_table = (u32*) (0xFFC00000 + (pd_index * 0x1000));

    page_table[pt_index] = 0;
    invlpg((u32) virt_addr);

    int empty = 1;
    for (int i = 0; i < 1024; i++) {
        if (page_table[i] != 0) {
            empty = 0;
            break;
        }
    }

    if (empty) {
        pframe_free(get_physical_addr(page_table));
        page_directory[pd_index] = 0x00000002;
    }
}

void unmap_first_mb() {
    u32* page_directory = (u32*) PAGE_DIRECTORY_VADDR;
    page_directory[0] = 0x00000002;
}

void init_heap() {
    init_pframe_allocator();
    unmap_first_mb();
    
    map_page(pframe_alloc(), (void*) HEAP_START, 3);
    g_heap_data.heap_end = (void *) (HEAP_START + 0x1000);
}

void* kmalloc(u32 size) {
    
}