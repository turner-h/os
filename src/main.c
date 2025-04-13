#include "vga.h"
#include "isr.h"
//#include "util.h"

void main() {
    clear_screen();
    install_isrs();

    
    asm volatile("int $1");
    asm volatile("int $2");
    asm volatile("int $3");
    
}