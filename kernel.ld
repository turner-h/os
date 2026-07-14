SECTIONS {
    . = 0xC0000000;

    .text : AT(0x00100000) {
        *(.text*)
    }

    .rodata : AT(ADDR(.rodata) - 0xC0000000 + 0x00100000) {
        *(.rodata*)
    }

    .data : AT(ADDR(.data) - 0xC0000000 + 0x00100000) {
        *(.data*)
    }
   
    .bss : AT(ADDR(.bss) - 0xC0000000 + 0x00100000) {
        *(.bss*)
        kernel_end = .;
    }
}