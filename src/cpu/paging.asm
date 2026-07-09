global enable_paging

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
