#include "idt.h"
#include "../lib/types.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_register;

gdt_entry_t gdt[GDT_ENTRIES];
gdt_register_t gdt_register;

void set_gdt() {
    gdt[0] = (gdt_entry_t) {0, 0, 0, 0, 0, 0};              // null segment
    gdt[1] = (gdt_entry_t) {0xFFFF, 0, 0, 0x9A, 0xCF, 0};   // kernel code segment
    gdt[2] = (gdt_entry_t) {0xFFFF, 0, 0, 0x92, 0xCF, 0};   // kernel data segment

    gdt_register.addr = (u32) &gdt;
    gdt_register.size = GDT_ENTRIES * sizeof(gdt_entry_t) - 1;
    asm volatile("lgdt (%0)" : : "r" (&gdt_register));

    reload_segments();
}

void set_idt_gate (int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].segment = KERNEL_CS;
    idt[n].zero = 0;
    idt[n].flags = 0x8E;
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    set_gdt();

    idt_register.base = (u32) &idt;
    idt_register.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    asm volatile("lidt (%0)" : : "r" (&idt_register));
}