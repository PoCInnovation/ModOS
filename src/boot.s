# /* Declare constants for the multiboot header. */
# .set ALIGN,         1<<0                            /* align loaded modules on page boundaries */
# .set MEMINFO,       1<<1                            /* provide memory map */
# .set VIDEO_MODE,    1<<2                            /* request graphics mode from multiboot */
# .set FLAGS,         ALIGN | MEMINFO | VIDEO_MODE    /* this is the Multiboot 'flag' field */
# .set MAGIC,         0xE85250D6                      /* 'magic number' lets bootloader find the header */
# .set CHECKSUM,      -(MAGIC + FLAGS)                /* checksum of above, to prove we are multiboot */
#
# .section .multiboot
# .align 8
# .long MAGIC
# .long FLAGS
# .long CHECKSUM
# .long 0, 0, 0, 0, 0
# .long 0 #set graphics mode
# .long 1024, 768, 32 #width, height, depth

/* Define constants for the Multiboot2 header fields. */
.set MAGIC,         0xE85250D6         /* Magic number to identify as Multiboot2 header */
.set ARCHITECTURE,  0x00               /* Architecture type (0x00 for 64-bit) */
.set HEADER_LENGTH, 20                 /* The length of the header (20 bytes: magic + architecture + length + checksum) */
.set CHECKSUM,      -(MAGIC + ARCHITECTURE + HEADER_LENGTH)  /* Checksum */

/* Define the Multiboot2 header. */
.section .multiboot
.align 8
.long MAGIC                /* Magic number (0xE85250D6) */
.long ARCHITECTURE         /* Architecture (0x00 for x86_64) */
.long HEADER_LENGTH        /* Header length (20 bytes) */
.long CHECKSUM             /* Checksum */

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.code32
.section .text
.global _start
.type _start, @function
_start:
    // TODO:
    // * A20 Line
    // * Check multiboot
    // * Check cpuid
    // * Check long mode
    // * Setup page tables
    // * Enable paging
    // * gdt
    // * Enter long mode

    movl $0x80000000, %eax    # Set the A-register to 0x80000000.
    cpuid                    # CPU identification.
    cmpl $0x80000001, %eax    # Compare the A-register with 0x80000001.
    jb .no_long_mode         # It is less, there is no long mode.

    // Disable Paging
    movl %cr0, %eax                                # Set the A-register to control register 0.
    and %eax, 0x7FFFFFFF                           # Clear the PG-bit, which is bit 31.
    movl %eax, %cr0                                # Set control register 0 to the A-register.

    movl $0x1000, %edi       # Set the destination index (EDI) to 0x1000
    movl %edi, %cr3          # Set control register CR3 to the destination index
    xorl %eax, %eax          # Nullify the EAX register
    movl $4096, %ecx         # Set ECX register to 4096
    rep stosl                # Clear the memory (EAX value to [EDI] repeatedly)
    movl %cr3, %edi

    movl $0x2003, (%edi)       # Set the 32-bit value at [EDI] to 0x2003
    addl $0x1000, %edi         # Add 0x1000 to the destination index (EDI)

    movl $0x3003, (%edi)       # Set the 32-bit value at [EDI] to 0x3003
    addl $0x1000, %edi         # Add 0x1000 to the destination index (EDI)

    movl $0x4003, (%edi)       # Set the 32-bit value at [EDI] to 0x4003
    addl $0x1000, %edi         # Add 0x1000 to the destination index (EDI)

    movl $0x00000003, %ebx          # Set the B-register (EBX) to 0x00000003
    movl $512, %ecx                 # Set the C-register (ECX) to 512

.SetEntry:
    movl %ebx, (%edi)           # Set the uint32_t at the destination index (EDI) to the value in EBX
    addl $0x1000, %ebx          # Add 0x1000 to the B-register (EBX)
    addl $8, %edi               # Add eight to the destination index (EDI)
    loop .SetEntry              # Loop back to .SetEntry, decreasing ECX until zero

    movl %cr4, %eax                 # Set the A-register (EAX) to the value of control register 4
    orl $(1 << 5), %eax             # Set the PAE bit (6th bit) in EAX
    movl %eax, %cr4                 # Set control register 4 to the updated A-register (EAX)

	mov $stack_top, %esp

    // Switch from real mode
    movl $0xC0000080, %ecx         # Set ECX to 0xC0000080, which is the EFER MSR
    rdmsr                          # Read from the model-specific register (MSR)
    orl $(1 << 8), %eax            # Set the LM-bit (9th bit, bit 8) in EAX
    wrmsr                          # Write to the model-specific register (MSR)

    movl %cr0, %eax                # Load control register CR0 into EAX
    orl $((1 << 31) | (1 << 0)), %eax  # Set the PG-bit (31st bit) and PM-bit (0th bit) in EAX
    movl %eax, %cr0                # Store the updated value back into control register CR0

    .code64
    cli                              # Clear the interrupt flag
    # movw $GDT64.Data, %ax            # Load the data descriptor into AX
    movw %ax, %ds                    # Set the data segment register (DS) to AX
    movw %ax, %es                    # Set the extra segment register (ES) to AX
    movw %ax, %fs                    # Set the FS segment register to AX
    movw %ax, %gs                    # Set the GS segment register to AX
    movw %ax, %ss                    # Set the stack segment register (SS) to AX
    movl $0xB8000, %edi              # Set the destination index (EDI) to 0xB8000
    movq $0x1F201F201F201F20, %rax   # Set the A-register (RAX) to 0x1F201F201F201F20
    movl $500, %ecx                  # Set the C-register (ECX) to 500

    movl %ebx, %ebx                 # Zero-extend EBX to RBX
    push %rbx                        # Push "addr" using the 64-bit RBX register

    movl %eax, %eax                 # Zero-extend EAX to RAX
    push %rax                        # Push "magic" using the 64-bit RAX register

	call kernel_main

	cli
1:	hlt
	jmp 1b

.no_long_mode:
2:	hlt
	jmp 2b

.size _start, . - _start
