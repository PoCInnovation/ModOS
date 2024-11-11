/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** kernel.c
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../include/debug.h"
#include "../include/kernel.h"
#include "../include/multiboot.h"
#include "../include/pixel.h"
#include "../include/config.h"
#include "../include/string.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__x86_64__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

uint8_t *framebuffer;
uint32_t framebuffer_pitch;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

// Kernel entry
void kernel_main(unsigned long magic, multiboot_info_t *mbi) {
    serial_init();
    serial_printf("This program is running on a %d-bit architecture.\n", (int)(sizeof(void*) * 8));
    serial_printf("Kernel loading\naddr: %x\nmagic: %x\nframebuffer_bpp: "
            "%d\nframebuffer_type: %d\n\n",
            (uintptr_t)mbi, magic, mbi->framebuffer_bpp,
            mbi->framebuffer_type);

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        serial_printstr("Bootloader error: magic isn't valid.");
        return;
    }

    if (!(mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO)) {
        serial_printstr("Graphics mode isn't enabled\n");
        return;
    }

    framebuffer = (uint8_t *)(uintptr_t)mbi->framebuffer_addr;
    framebuffer_pitch  = mbi->framebuffer_pitch;
    framebuffer_width  = mbi->framebuffer_width;
    framebuffer_height = mbi->framebuffer_height;
    framebuffer_bpp    = mbi->framebuffer_bpp;

    clear_screen(BACKGROUND_COLOR);

    printf("Hello %d\n", 42);
}
