#ifndef PORT_H
#define PORT_H

#define SERIAL_PORT 0x3f8

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);

void sprintf(char* format, ...);

#endif
