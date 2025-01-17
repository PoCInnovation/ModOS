section .multiboot
    align 8

multiboot_header:
    ; Magic number for Multiboot v2
    dd 0xE85250D6

    ; Architecture (0x0 = i386, 0x1 = MIPS)
    dd 0x0

    ; Total header length (including the end tag)
    dd header_end - multiboot_header

    ; Checksum (magic + architecture + checksum == 0)
    dd -(0xE85250D6 + 0x0 + (header_end - multiboot_header))

; End tag to mark the end of the Multiboot header
multiboot_header_end:
    dw 0
    dw 0
    dd 0
header_end:

