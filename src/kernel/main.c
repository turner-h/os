#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"

void main() {
    clear_screen();
    install_isrs();

    asm volatile("sti");
    init_keyboard();
}