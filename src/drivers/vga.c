#include "vga.h"
#include "../cpu/port.h"
#include "../lib/util.h"

int get_cursor_pos() {
    port_byte_out(SCREEN_CTRL, 14);             // request high byte
    int position = port_byte_in(SCREEN_DATA) << 8;   // read high byte
    port_byte_out(SCREEN_CTRL, 15);             // request low byte
    position += port_byte_in(SCREEN_DATA);      // read low byte

    return position * 2;
}

void set_cursor_pos(int position) {             //inverse of get_cursor_pos()
    position /= 2;
    port_byte_out(SCREEN_CTRL, 14);
    port_byte_out(SCREEN_DATA, (unsigned char)(position >> 8));
    port_byte_out(SCREEN_CTRL, 15);
    port_byte_out(SCREEN_DATA, (unsigned char)(position & 0xff));
}

int get_offset(int x, int y) {
    return 2 * ((y * VGA_WIDTH) + x);
}

int get_offset_row(int offset) {
    return offset / (2 * VGA_WIDTH);
}

void kprint_at(int x, int y, char* message) {
    // (-1, -1) uses current cursor position
    int offset;
    if (x < 0 && y < 0) {
        offset = get_cursor_pos();
    } else {
        offset = get_offset(x, y);
    }

    char *screen = (char*) VGA_ADDRESS;

    int i = 0;
    while (message[i] != 0) {
        if (offset > get_offset(VGA_WIDTH-1, VGA_HEIGHT-1)) {   //scrolling
            for(int i = 0; i < VGA_HEIGHT; i++) {
                memory_copy((char*) (VGA_ADDRESS + get_offset(0, i+1)), (char*) (VGA_ADDRESS + get_offset(0, i )), 2 * (VGA_WIDTH));
            }

            offset = get_offset(0, VGA_HEIGHT-1);
        }

        if (message[i] == '\n')  {
            int row = get_offset_row(offset);
            offset = get_offset(0, row + 1);
        } else {
            screen[offset] = message[i];
            screen[offset+1] = 0x0f;        // white text, black background

            offset += 2;
        }

        i++;
    }
    set_cursor_pos(offset);
}

void kprint(char *message) {
    kprint_at(-1, -1, message);
}

void clear_screen() {
    int buffer_size = VGA_WIDTH * VGA_HEIGHT;
    char *screen = (char*) VGA_ADDRESS;

    for (int i = 0; i < buffer_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = 0x0f;
    }

    set_cursor_pos(get_offset(0,0));
}

