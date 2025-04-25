[extern isr_handler]    ; defined in isr.c

isr_common:
    ; save cpu state so registers_t can read it
    pusha   ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds
    push eax    ; save the data segment descriptor
    mov ax, 0x10    ; kernel code segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; call c handler
    call isr_handler

    ; reset cpu state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8  ; clean up error code and int number
    sti

    iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common

isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common

isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common

isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common

isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common

isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common

isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common

isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common

isr8:   ; has error code
    cli
    push byte 8
    jmp isr_common

isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common

isr10:  ; has error code
    cli
    push byte 10
    jmp isr_common

isr11:  ; has error code
    cli
    push byte 11
    jmp isr_common

isr12:  ; has error code
    cli
    push byte 12
    jmp isr_common

isr13:  ; has error code
    cli
    push byte 13
    jmp isr_common

isr14:  ; has error code
    cli
    push byte 14
    jmp isr_common

isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common

isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common

isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common

isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common

isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common

isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common

isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common

isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common

isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common

isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common

isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common

isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common

isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common

isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common

isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common

isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common

isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common

irq0:
    cli
    push byte 0
    push byte 32
    jmp isr_common

irq1:
    cli
    push byte 1
    push byte 33
    jmp isr_common

irq2:
    cli
    push byte 2
    push byte 34
    jmp isr_common

irq3:
    cli
    push byte 3
    push byte 35
    jmp isr_common

irq4:
    cli
    push byte 4
    push byte 36
    jmp isr_common

irq5:
    cli
    push byte 5
    push byte 37
    jmp isr_common

irq6:
    cli
    push byte 6
    push byte 38
    jmp isr_common

irq7:
    cli
    push byte 7
    push byte 39
    jmp isr_common

irq8:
    cli
    push byte 8
    push byte 40
    jmp isr_common

irq9:
    cli
    push byte 9
    push byte 41
    jmp isr_common

irq10:
    cli
    push byte 10
    push byte 42
    jmp isr_common

irq11:
    cli
    push byte 11
    push byte 43
    jmp isr_common

irq12:
    cli
    push byte 12
    push byte 44
    jmp isr_common

irq13:
    cli
    push byte 13
    push byte 45
    jmp isr_common

irq14:
    cli
    push byte 14
    push byte 46
    jmp isr_common

irq15:
    cli
    push byte 15
    push byte 47
    jmp isr_common