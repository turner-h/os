#include "../lib/types.h"

#define VGA_ADDRESS 0xb8000
#define SCREEN_CTRL 0x3D4
#define SCREEN_DATA 0x3D5

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

void clear_screen();
void kprint(char *message);
void kprint_at(int x, int y, int len, char *message);
void kprint_char(char *message);
void kprint_u8(u8 num);
void kprint_len(char* message, int len);