#include "vga.h"

void main() {
    clear_screen();

    for (int i = 0; i < 28; i++) {
        for (int j = 0; j < i; j++) {
            kprint("x");
        }
        kprint("\n");
    }
}