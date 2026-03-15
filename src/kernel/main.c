#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../kernel/fs.h"
#include "../lib/util.h"

void main() {
    clear_screen();
    install_isrs();

    asm volatile("sti");
    init_keyboard();

    init_fs();

    u16 buffer[256];
    fs_open("/test/test.txt", (u16*) buffer);

    kprint((char*) buffer);
    kprint("\n");

    fs_open("/test/test2/woah.txt", (u16*) buffer);
    kprint((char*) buffer);
}