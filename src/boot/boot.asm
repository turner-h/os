[bits 16]
[org 0x7c00]

;jmp short bootloader
;nop

;times 87 db 0

bootloader:

STAGE_2_OFFSET equ 0x10000
MMAP_ENTRIES equ 0x8000
MMAP_LOCATION equ 0x8004
SMAP equ 0x534D4150

mov [BOOT_DRIVE], dl

mov bp, 0x9000  ; init stack
mov bp, sp

in al, 0x92         ; enable A20 line
or al, 2
out 0x92, al

call read_root_dir

STAGE_2_SIZE_OFFSET equ 28

call load_kernel
call detect_memory
call switch_to_protected_mode

jmp $

read_root_dir:
    mov bx, sector_2        ; reads to sector 2
    mov dh, 1               ; reads 1 sector
    mov dl, [BOOT_DRIVE]    ; drive to load from
    mov cl, 0x22            ; sector to read, 0x01 is the boot sector
    call disk_load
    ret

load_kernel:
    mov ax, 0x1000      ; sets kernel address to 0x10000
    mov es, ax
    xor bx, bx

    mov ax, [sector_2 + STAGE_2_SIZE_OFFSET] ; get kernel size
    shr ax, 7

    mov dh, al           ; # of sectors to read
    mov dl, [BOOT_DRIVE]    ; drive to load from
    mov cl, 0x23    ; sector to read, 0x01 is the boot sector
    call disk_load
    ret

disk_load:
    pusha
    push dx

    mov ah, 0x02    ; set int 13h to 'read'
    mov al, dh      ; number of sectors to read
    mov ch, 0       ; cylinder

    ; dl is the drive number
    mov dh, 1       ; head

    int 0x13
    jc disk_error

    pop dx
    cmp al, dh      ; al is set ot the # of sectors read
    jne disk_error
    popa
    ret

disk_error:
    mov bx, DRIVE_ERROR
    call print_rm
    jmp $

print_rm:
    pusha

print_rm_start:
    mov al, [bx]    ; bx is string address
    cmp al, 0
    je print_rm_end

    mov ah, 0x0e
    int 0x10

    add bx, 1
    jmp print_rm_start

print_rm_end:
    popa
    ret

detect_memory:
    xor ax, ax
    mov es, ax
    mov di, MMAP_LOCATION   ; set mmap buffer location in es:di
   
    xor bp, bp              ; store entry count in bp

    xor ebx, ebx            ; function call for int 0x15, eax 0xe280
    mov edx, SMAP
    mov eax, 0xE820
    mov [es:di + 20], dword 1   ; forces a valid entry somehow?
    mov ecx, 24
    int 0x15

    jc dm_failed            ; test failure cases
    mov edx, SMAP
    cmp eax, edx
    jne dm_failed
    test ebx, ebx
    je dm_failed
   
dm_loop:
    mov eax, 0xE820
    mov [es:di + 20], dword 1   ; forces a valid entry somehow?
    mov ecx, 24
    int 0x15

    jc dm_finished
    mov edx, SMAP

dm_jump_in:
    jcxz dm_skip_entry
    cmp cl, 20
    jbe dm_not_ext

    test byte [es:di + 20], 1   ; checks the "ignore this entry" bit
    jmp dm_skip_entry           ; why would you want a bit for that 

dm_not_ext:     
    mov ecx, [es:di + 8]
    or ecx, [es:di + 12]    ; checks that memory section length != 0
    jz dm_skip_entry

    inc bp
    add di, 24

dm_skip_entry:
    test ebx, ebx
    jne dm_loop

dm_finished:
    mov [es:MMAP_ENTRIES], bp
    clc
    ret
dm_failed:
    stc 
    ret

switch_to_protected_mode:
    cli                     ; clear interrupts
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

    mov ebx, PM_MESSAGE
    call print

    call STAGE_2_OFFSET  ; jump to C
    jmp $               ; loop infinitely if kernel returns

BOOT_DRIVE db 0
DRIVE_ERROR db "failed to load from drive", 0
PM_MESSAGE db "loaded in protected mode", 0
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

sector_2:
