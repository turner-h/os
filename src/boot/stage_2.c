#include "../lib/types.h"

#define ROOT_DIR_ADDR 0xC000
#define KERNEL_PHYS_ADDR 0x00100000
#define KERNEL_VADDR 0xC0000000

extern void enable_paging(unsigned int *page_directory);
extern void update_stack_pointer();
extern void flush_tlb();
extern u32 boot_end;

u32 pages_allocated;
u32* b_first_page_addr;

u32* b_pframe_alloc() {
    u8* addr = (u8*) (b_first_page_addr + (pages_allocated * 0x00001000));
    pages_allocated++;
    return (u32*) addr; 
}

int b_map_page(void* phys_addr, void* virt_addr, int flags) {
    u32 pd_index = (u32) virt_addr >> 22;
    u32 pt_index = ((u32) virt_addr >> 12) & 0x03FF;
    
    u32* page_directory = (u32*) 0xFFFFF000;
    if (!(page_directory[pd_index] & 0x1)) {   // pt doesnt exist
        u32* page_table = b_pframe_alloc();
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

void remap_kernel(u32 kernel_pages) {
    int i;
    for (i = 0; i < kernel_pages + 1; i++) {
        b_map_page((void*) (KERNEL_PHYS_ADDR + (i * 0x1000)), (void*) (KERNEL_VADDR + (i * 0x1000)), 3);
    }

    for (int j = 0; j < 10; j++) {
        b_map_page((void *) b_pframe_alloc(), (void *) (0xC0008000 - (j * 0x1000)), 3);
    }

    b_map_page((void *) 0x000b8000, (void *) 0xC00B8000, 3);

    flush_tlb();
}

void init_paging() {
    b_first_page_addr = (u32 *) ((((u32) &boot_end >> 12) + 1) << 12);  // gets next frame after bootloader
    pages_allocated = 0;

    u32* page_directory = b_pframe_alloc();
    u32* first_page_table = b_pframe_alloc();

    for (unsigned int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002;                 // kernel only, R/W enabled, page table is not present
        first_page_table[i] = (i * 0x1000) | 0x13;      // identity maps first 4Mib, kernel only, R/W enabled, page is present
    }

    page_directory[0] = ((u32) first_page_table) | 3;   // kernel only, R/W, present
    page_directory[1023] = (u32) page_directory | 3;    // map last pde to itself

    enable_paging(page_directory);
}

unsigned char port_byte_in(unsigned short port) {
    char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

void ata_400ns() {  // 400ns delay, lets the drive reset properly
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
}

void block_until_ready() {
    while(port_byte_in(0x1F7) & 0x80);
    while(!(port_byte_in(0x1F7) & 0x08));
}

int disk_read(int slave, u32 addr, u32 sectors, u16 *buffer) {
    port_byte_out(0x1F6, 0xE0 | (slave << 4) | ((addr >> 24) & 0x0F));       // select drive
    ata_400ns();   
    
    port_byte_out(0x1F2, (u8) sectors);     
    port_byte_out(0x1F3, (u8) (addr));
    port_byte_out(0x1F4, (u8) (addr >> 8));
    port_byte_out(0x1F5, (u8) (addr >> 16));
    port_byte_out(0x1F7, 0x20);             // ATA PIO command
    ata_400ns();

    for (int i = 0; i < sectors; i++) {
        block_until_ready();                    // waits until disk is ready

        for (int j = 0; j < 256; j++) {
            u16 data = port_word_in(0x1F0);
            buffer[(i * 256) + j] = data;
        }

        port_byte_in(0x1F7);
        ata_400ns();
    }

    return 0;
}

u32 load_kernel() {
    u8 root_dir[512];
    disk_read(0, ROOT_DIR_ADDR >> 9, 1, (u16*) &root_dir);

    u32 kernel_size = *(u32 *)(root_dir + 60);
    u32 kernel_sectors = (kernel_size + 511) / 512;
    u32 kernel_pages = (kernel_size + 4095) / 4096;

    u8 boot_sector[512];
    disk_read(0, 6, 1, (u16*) &boot_sector);
    u8 sectors_per_cluster = boot_sector[13];
    u16 reserved_sectors = *(u16 *)(boot_sector + 14);
    u8 number_of_fats = boot_sector[16];
    u32 sectors_per_fat = *(u32 *)(boot_sector + 36);
   
    u32 first_data_sector = reserved_sectors + (number_of_fats * sectors_per_fat);
    
    u32 kernel_disk_cluster = (((u32) (*(u16 *)(root_dir + 52))) << 16) + *(u16 *)(root_dir + 58);
    u32 kernel_disk_addr = first_data_sector + ((kernel_disk_cluster - 2) * sectors_per_cluster);
    
    u16* kernel = (u16*) KERNEL_PHYS_ADDR;
    disk_read(0, kernel_disk_addr, kernel_sectors, kernel);
    
    return kernel_pages;
}

void start() {
    u32 kernel_pages = load_kernel();
    init_paging();
    remap_kernel(kernel_pages);

    update_stack_pointer();
}

void main_jump() {
    ((void (*)(void)) KERNEL_VADDR)();
}