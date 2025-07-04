#include "ata.h"
#include "../cpu/port.h"
#include "../drivers/vga.h"
#include "../cpu/isr.h"
#include "../lib/util.h"

void ata_400ns() {  // 400ns delay, lets the drive reset properly
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
}

void block_until_ready() {
    while(port_byte_in(0x1F7) & 0x80);
    while(!(port_byte_in(0x1F7) & 0x08));
}

int disk_read(int slave, u32 addr, u32 sectors, u16 *buffer) {
    port_byte_out(0x1F6, 0xE0 | (slave << 4) | ((addr >> 24) & 0x0F));       // select drive
    ata_400ns();   
    
    port_byte_out(0x1F2, (u8) sectors);     
    port_byte_out(0x1F3, (u8)(addr));
    port_byte_out(0x1F4, (u8)(addr >> 8));
    port_byte_out(0x1F5, (u8)(addr >> 16));
    port_byte_out(0x1F7, 0x20);             // ATA PIO command
    ata_400ns();

    block_until_ready();                    // waits until disk is ready

    for (u8 i = 0; i < 255; i++) {
        u16 data = port_word_in(0x1F0);
        buffer[i] = data;
    }

    return 0;
}
