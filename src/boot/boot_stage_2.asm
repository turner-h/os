[bits 32]
[extern start]
[extern main_jump]

call start
jmp $

global enable_paging
global update_stack_pointer
global flush_tlb

enable_paging:
    push ebp
    mov ebp, esp

    mov eax, [esp + 8]  ; gets page_directory argument
    mov cr3, eax

    mov eax, cr0
    or eax, 0x80000001  ; set paging and protection bits of cr0
    mov cr0, eax

    mov esp, ebp
    pop ebp
    ret

update_stack_pointer:
    mov esp, 0xC0009000
    mov ebp, 0xC0009000
    call main_jump

flush_tlb:
    mov eax, cr3
    mov cr3, eax
    ret