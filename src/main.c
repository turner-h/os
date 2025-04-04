#include "port.h"

void main() {
    port_byte_out(0x3D4, 14);
    int position = port_byte_in(0x3D5);
    position << 8;

    port_byte_out(0x3D4, 15);
    position += port_byte_in(0x3D5);

    int offset = position * 2;

    char* vga_buffer = (char*) 0xb8000;
    vga_buffer[offset + 2] = 'X';
    vga_buffer[offset + 3] = 0x0f;
}