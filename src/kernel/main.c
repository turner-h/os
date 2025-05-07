#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/ata.h"

void main() {
    clear_screen();
    install_isrs();

    asm volatile("sti");
    init_keyboard();

    u8 buffer[512];

    disk_read(0, 512, 0, 1, (u16*) buffer);
    kprint(buffer);
}