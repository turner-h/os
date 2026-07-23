#include "../cpu/isr.h"
#include "../cpu/memory.h"
#include "../drivers/keyboard.h"
#include "../drivers/vga.h"
#include "../drivers/ata.h"
#include "../kernel/fs.h"
#include "../lib/util.h"

void main() {
    clear_screen();
    kprintf("higher half kernel loaded\n");
    install_isrs();
    init_heap();

    asm volatile("sti");
    init_keyboard();

    init_fs();

    u16* buffer = kmalloc(256 * sizeof(u16));
    fs_open("/test.txt", buffer);

    kprintf((char*) buffer);
    kprintf("\n");

    fs_open("/test2/woah.txt", buffer);
    kprintf((char*) buffer);
    kprintf("\n");

    kfree(buffer);
}
