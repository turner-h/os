#include "../cpu/isr.h"
#include "../cpu/memory.h"
#include "../drivers/keyboard.h"
#include "../drivers/vga.h"
#include "../kernel/fs.h"
#include "../test/test.h"
#include "../cpu/port.h"

void main() {
    clear_screen();
    kprintf("higher half kernel loaded\n");
    install_isrs();
    init_heap();

    asm volatile("sti");
    init_keyboard();

    init_fs();

#ifdef TEST
    sprintf("\n== OS Unit Tests ==\n");

    init_tests();
    test_run_all();
#endif

    u16* buffer = kmalloc(256 * sizeof(u16));
    fs_open("/test.txt", buffer);

    kprintf((char*) buffer);
    kprintf("\n");

    fs_open("/test2/woah.txt", buffer);
    kprintf((char*) buffer);
    kprintf("\n\n");

    kfree(buffer);
}
