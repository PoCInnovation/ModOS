/* Declare constants for the multiboot header. */
.set ALIGN,         1<<0                            /* align loaded modules on page boundaries */
.set MEMINFO,       1<<1                            /* provide memory map */
.set VIDEO_MODE,    1<<2                            /* request graphics mode from multiboot */
.set FLAGS,         ALIGN | MEMINFO | VIDEO_MODE    /* this is the Multiboot 'flag' field */
.set MAGIC,         0xE85250D6                      /* 'magic number' lets bootloader find the header */
.set CHECKSUM,      -(MAGIC + FLAGS)                /* checksum of above, to prove we are multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0, 0, 0, 0, 0
.long 0 #set graphics mode
.long 1024, 768, 32 #width, height, depth

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
    // TODO:
    // * A20 Line
    // * Setting up paging
    // * Enter long mode

	mov $stack_top, %esp

    push %ebx // Push "addr"
    push %eax // Push "magic"

	call kernel_main

	cli
1:	hlt
	jmp 1b

.size _start, . - _start
