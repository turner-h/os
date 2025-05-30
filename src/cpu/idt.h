#ifndef IDT_H
#define IDT_H

#include "../lib/types.h"

#define KERNEL_CS 0x08

typedef struct {
    u16 low_offset;
    u16 segment;
    u8 zero;
    u8 flags;
    u16 high_offset;
}__attribute__((packed)) idt_gate_t;

typedef struct {
    u16 limit;  // size
    u32 base;   // address
}__attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256 
extern idt_gate_t idt[IDT_ENTRIES];
extern idt_register_t idt_register;

void set_idt_gate (int n, u32 handler);
void set_idt();

#endif