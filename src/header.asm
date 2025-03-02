section .multiboot_header
header_start:
    dd 0xe85250d6
    dd 0 ; Protected mode
    dd header_end - header_start
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))

bootinfo_request_tag:
    dd 1
    dd bootinfo_request_tag_end - bootinfo_request_tag
    dd 6    ; memory map
    dd 1    ; boot command line
    dd 8    ; framebuffer info
    dd 12   ; efi64 system table pointer
bootinfo_request_tag_end:

framebuffer_tag_start:
    dw 5
    dw 1
    dd framebuffer_tag_end - framebuffer_tag_start
    dd 1024
    dd 768
    dd 32

align_tag:
    dd 0  ; Type = 0 (end tag)
    dd 8  ; Size of the tag (8 bytes)
framebuffer_tag_end:
header_end:
