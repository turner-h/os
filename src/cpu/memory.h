#ifndef MEMORY_H
#define MEMORY_H

#include "../lib/types.h"

#define PAGE_DIRECTORY_VADDR 0xFFFFF000

typedef struct {
    u32 size;
    struct FreeNode* next;
} FreeNode;

typedef struct {
    u32* heap_addr;
    u32* heap_end;
    FreeNode* free_node;
} HeapData;

extern u32 kernel_end;

void init_pframe_allocator();
void* get_physical_addr(void* virt_addr);
int map_page(void* phys_addr, void* virt_addr, int flags);
void unmap_page(void* virt_addr);

static inline void invlpg(u32 virt_addr) {
    asm volatile("invlpg (%0)" :: "r" (virt_addr) : "memory");
}

#endif