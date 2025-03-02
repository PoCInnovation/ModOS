global long_mode_start
extern kmain

section .text
bits 64
long_mode_start:
    ; mov rsi, rdi    ; Info pointer to second argument (rsi)
    ; mov rdi, rsi    ; Magic number to first argument (rdi)

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kmain

    hlt
