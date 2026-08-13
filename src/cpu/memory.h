#ifndef MEMORY_H
#define MEMORY_H

#include "../lib/types.h"

// one million pages!!!!
#define TOTAL_PAGES 1048576
#define PAGE_DIRECTORY_VADDR 0xFFFFF000
#define HEAP_START 0xC0010000
#define MMAP_ENTRIES_ADDR 0x8000
#define MMAP_ADDR 0x8004

extern u32 kernel_end;

struct FreeNode {
    u32 size;
    struct FreeNode* next;
    struct FreeNode* prev;
};

typedef struct {
    void* heap_end;
    struct FreeNode* free_node;
} HeapData;

typedef struct {
    u64 base_address;
    u64 length;
    u32 region_type;
    u32 attributes;
} __attribute__((packed)) MmapEntry; 

void init_heap();
void* get_physical_addr(void* virt_addr);
int map_page(void* phys_addr, void* virt_addr, int flags);
void unmap_page(void* virt_addr);

void* kmalloc(u32 size);
void kfree(void* addr);

static inline void invlpg(u32 virt_addr) {
    asm volatile("invlpg (%0)" :: "r" (virt_addr) : "memory");
}

#endif
