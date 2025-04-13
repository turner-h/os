#include "idt.h"
#include "types.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_register;

void set_idt_gate (int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].segment = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_register.base = (u32) &idt;
    idt_register.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_register));
}