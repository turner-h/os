#include "../cpu/port.h"
#include "../lib/types.h"
#include "../lib/util.h"

unsigned char port_byte_in(unsigned short port) {
    char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

int serial_transmit_empty() {
    return (SERIAL_PORT + 5) & 0x20;
}

void serial_write_char(unsigned char ch) {
    while (serial_transmit_empty() == 0);
    port_byte_out(SERIAL_PORT, ch);
}

void serial_write(const char* s) {
    while (*s) {
        serial_write_char((unsigned char) *s++);
    }
}

void sprintf(char* format, ...) {
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
                    serial_write(s);
                    i++;
                    break;
                    
                case 's':
                    char* string = __builtin_va_arg(args, char*);
                    serial_write(string);
                    i++;
                    break;

                case 'x':
                    u32 hex = __builtin_va_arg(args, u32);
                    serial_write("0x");

                    char h[3];
                    char hex_byte = 4;
                    for (int j = 0; j < 4; j++) {
                        hex_byte = (hex >> ((3 - j) * 8)) & 0xFF;
                        cth(hex_byte, h);
                        serial_write(h);
                    }
                    i++;
                    break;
                    
                default:
                    serial_write_char((char) *(format + i));
                    break;
            }
        } else {
            serial_write_char((char) *(format + i));
        }
        i++;
    }

    __builtin_va_end(args);
}
