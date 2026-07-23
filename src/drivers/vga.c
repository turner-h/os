#include "vga.h"
#include "../cpu/port.h"
#include "../lib/util.h"

char g_print_color = 0x0f;

int char_in_message(char* message, int i, int len);

int get_cursor_pos() {
    port_byte_out(SCREEN_CTRL, 14);                 // request high byte
    int position = port_byte_in(SCREEN_DATA) << 8;  // read high byte
    port_byte_out(SCREEN_CTRL, 15);                 // request low byte
    position += port_byte_in(SCREEN_DATA);          // read low byte

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

void kprint_at(int x, int y, int len, char* message) {
    // (-1, -1) uses current cursor position
    int offset;
    if (x < 0 && y < 0) {
        offset = get_cursor_pos();
    } else {
        offset = get_offset(x, y);
    }

    char *screen = (char*) VGA_ADDRESS;

    int i = 0;
    while (char_in_message(message, i, len)) {
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
            screen[offset+1] = g_print_color;        // white text, black background

            offset += 2;
        }

        i++;
    }
    set_cursor_pos(offset);
}

int char_in_message(char* message, int i, int len) {
    if (len < 0) {
        return message[i] != 0;
    } else {
        return i < len;
    }
}

void kprint(char *message) {
    kprint_at(-1, -1, -1, message);
}

void kprint_char(char *message) {
    char printed_char = *message;
    kprint(&printed_char);
}

void kprint_u8(u8 num) {
    char str[4];
    itoa(num, str);
    kprint(str);
}

void kprint_len(char* message, int len) {
    kprint_at(-1, -1, len, message);
}

void kprintf(char* format, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, format);

    int i = 0;
    while (format[i] != 0) {
        if (format[i] == '%') {
            switch (format[i + 1]) {
                case 'd':
                    int digit = __builtin_va_arg(args, int);
                    char s[3];
                    itoa(digit, s);
                    kprint(s);
                    i++;
                    break;
                    
                case 's':
                    char* string = __builtin_va_arg(args, char*);
                    kprint(string);
                    i++;
                    break;

                case 'x':
                    u32 hex = __builtin_va_arg(args, u32);
                    kprint("0x");

                    char h[3];
                    char hex_byte = 4;
                    for (int j = 0; j < 4; j++) {
                        hex_byte = (hex >> ((3 - j) * 8)) & 0xFF;
                        cth(hex_byte, h);
                        kprint(h);
                    }
                    i++;
                    break;
                    
                default:
                    kprint_char(format + i);
                    break;
            }
        } else {
            kprint_char(format + i);
        }
        i++;
    }

    __builtin_va_end(args);
}

void vga_set_color(u8 foreground, u8 background) {
    g_print_color = ((background << 4) & 0xF0) + (foreground & 0xF);
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

