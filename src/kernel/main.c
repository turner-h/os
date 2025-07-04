#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"
#include "../lib/util.h"

void main() {
    clear_screen();
    install_isrs();

    asm volatile("sti");
    init_keyboard();

    u16 buffer[256];

    disk_read(0, 0, 1, (u16*) buffer);
    
    for (int i = 0; i < 256; i++) {
        u16 data = buffer[i];
        char hex[3];

        u8 h = data & 0xFF;
        cth(h, hex);
        kprint(hex);
        kprint(" ");

        h = data >> 8;
        cth(h, hex);
        kprint(hex);
        kprint(" ");
    }
}