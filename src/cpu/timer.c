#include "timer.h"
#include "port.h"
#include "isr.h"

#define BASE_FREQ 1193182

void pic_isr(registers_t reg) {
    port_byte_out(0x20, 0x20);
}

void init_pic(int hz) {
    int divisor = BASE_FREQ / hz;
    port_byte_out(0x43, 0x36);      // command byte
    port_byte_out(0x40, divisor & 0xFF);
    port_byte_out(0x40, divisor >> 8);

    register_interrupt_handler(32, pic_isr);
}
