[bits 16]
[org 0x7c00]

mov bp, 0x9000  ; init stack
mov bp, sp

; load kernel into RAM
call switch_to_protected_mode

jmp $

switch_to_protected_mode:
    cli                     ; clear interrupta
    lgdt [gdt_descriptor]   ; load gdt
    mov eax, cr0
    or eax, 0x1             ; enable protected mode
    mov cr0, eax
    jmp CODE_SEG:init_pm    ; jump to 32 bits, flushes the cpu of any 16 bit instructions

[bits 32]
init_pm:
    mov ax, DATA_SEG    ; set up segment registers (might be initialized with garbage data)
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000    ; set up 32-bit stack
    mov esp, ebp

    mov ebx, pm_message
    call print

jmp $   ; loop infinitely

pm_message:
    db "loaded in protected mode", 0

VIDEO_MEMORY equ 0xb8000    ; location of vga buffer
WHITE_ON_BLACK equ 0x0f     ; color scheme

print:
    pusha
    mov edx, VIDEO_MEMORY

print_loop:
    mov al, [ebx]   ; [ebx] is the character
    mov ah, WHITE_ON_BLACK

    cmp al, 0       ; check if string ends
    je print_done

    mov [edx], ax   ; moves char to vga buffer
    add ebx, 1      ; increment character
    add edx, 2      ; increment buffer index

    jmp print_loop

print_done:
    popa
    ret

gdt_start:
    dq 0   

gdt_code:
    dw 0xffff       ; segment length
    dw 0            ; segment base address
    db 0       
    db 0b10011010   ; flags
    db 0b11001111
    db 0            ; more segment base (data's all split up for some reason)

gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 0b10010010
    db 0b11001111
    db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size, 16 bits
    dd gdt_start                ; address, 32 bits

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510-($-$$) db 0
db 0x55, 0xAA