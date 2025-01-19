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
void kmain(void *mbi, unsigned long magic) {
    serial_init();
    serial_printf("Hello %d-bit, world!\n", (int)(sizeof(void*) * 8));
    serial_printf("Magic: %x\n", magic);
    serial_printf("Addr: %x\n", mbi);
    // serial_printf("framebuffer_addr: %x\n", mbi->framebuffer_addr);

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        serial_printstr("Bootloader error: magic isn't valid.");
        return;
    }

    // framebuffer = (uint8_t *)(uintptr_t)mbi->framebuffer_addr;
    // framebuffer_pitch  = mbi->framebuffer_pitch;
    // framebuffer_width  = mbi->framebuffer_width;
    // framebuffer_height = mbi->framebuffer_height;
    // framebuffer_bpp    = mbi->framebuffer_bpp;

    clear_screen(BACKGROUND_COLOR);

    printf("Hello %d\n", 42);
}
