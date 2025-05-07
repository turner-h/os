#include "ata.h"
#include "../cpu/port.h"

void ata_400ns() {  // 400ns delay, lets the drive reset properly
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
    port_byte_in(0x3F6);
}

void block_until_ready() {
    while(1) {
        u8 status = port_byte_in(0x1F7);
        if (!(status & 0x80) && (status & 0x08)){
            break;
        }
    }
}

int disk_read(int slave, u16 max_size, u32 addr, u32 sectors, u16 *buffer) {
    u8 read_cmd[12] = {0xA8, 0, (addr >> 0x18) & 0xFF, (addr >> 0x10) & 0xFF, (addr >> 0x08) & 0xFF, addr & 0xFF, (sectors >> 0x18) & 0xFF, (sectors >> 0x10) & 0xFF, (sectors >> 0x08) & 0xFF, sectors & 0xFF, 0, 0};
    u8 *cmd_ptr = read_cmd;

    port_byte_out(0x1F6, slave << 4);       // select drive
    ata_400ns();   
    port_byte_out(0x1F1, 0x0);              // port io mode
    port_byte_out(0x1F4, max_size & 0xFF);  // max_size low byte
    port_byte_out(0x1F5, max_size >> 8);    // max_size high byte
    port_byte_out(0x1F7, 0xA0);             // ATAPI command

    block_until_ready();                    // waits until disk is ready

    port_word_out(0x1F0, *cmd_ptr);         // send command packet
    port_word_out(0x1F0, *(cmd_ptr + 2));
    port_word_out(0x1F0, *(cmd_ptr + 4));
    port_word_out(0x1F0, *(cmd_ptr + 6));
    port_word_out(0x1F0, *(cmd_ptr + 8));
    port_word_out(0x1F0, *(cmd_ptr + 10));

    for (u32 i = 0; i < sectors; i++) {
        block_until_ready();

        int size = (port_byte_in(0x1F5) << 8) | port_byte_in(0x1F4);

        for (int j = 0; j < size / 2; j += 2) {
            buffer[(i * size/2) + j] = port_word_in(0x1F0);
        }
    }
}