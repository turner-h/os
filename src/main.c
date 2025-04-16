#include "vga.h"
#include "isr.h"

//#include "util.h"

static void timer(registers_t r) {
    kprint(".");
}

void main() {
    clear_screen();
    install_isrs();

    asm volatile("sti");
    register_interrupt_handler(32, timer);
}