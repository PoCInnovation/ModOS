#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__x86_64__)
#error "This kernel needs to be compiled with an x86_64-elf compiler"
#endif

#define MULTIBOOT2_BOOTLOADER_MAGIC 0x36d76289

uint8_t *framebuffer;
uint32_t framebuffer_pitch;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

/* Serial output (COM1) for debugging */
#define COM1_PORT 0x3F8

static inline uint8_t inb(uint16_t port)
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void serial_init()
{
    outb(COM1_PORT + 1, 0x00); /* Disable interrupts */
    outb(COM1_PORT + 3, 0x80); /* Enable DLAB (set baud rate divisor) */
    outb(COM1_PORT + 0, 0x03); /* Set divisor to 3 (low byte) 38400 baud */
    outb(COM1_PORT + 1, 0x00); /*                  (high byte) */
    outb(COM1_PORT + 3, 0x03); /* 8 bits, no parity, one stop bit */
    outb(COM1_PORT + 2, 0xC7); /* Enable FIFO, clear them, with 14-byte threshold */
    outb(COM1_PORT + 4, 0x0B); /* IRQs enabled, RTS/DSR set */
}

void serial_wait_for_ready()
{
    while ((inb(COM1_PORT + 5) & 0x20) == 0); /* Wait for the transmit holding register to be empty */
}

int putchar(int c)
{
    serial_wait_for_ready();
    outb(COM1_PORT, c);
    return c;
}

void serial_printstr(const char *str)
{
    while (*str) {
        putchar(*str);
        str++;
    }
    putchar('\n');
}

void serial_print_hex(uint64_t value)
{
    const char *hex_digits = "0123456789ABCDEF";
    for (int i = 15; i >= 0; i--) {
        putchar(hex_digits[(value >> (i * 4)) & 0xF]);
    }
    putchar('\n');
}

void clear_screen(uint32_t color)
{
    for (uint32_t y = 0; y < framebuffer_height; y++) {
        for (uint32_t x = 0; x < framebuffer_width; x++) {
            uint32_t *pixel = (uint32_t *)(framebuffer + y * framebuffer_pitch + x * (framebuffer_bpp / 8));
            *pixel = color;
        }
    }
}

void draw_line()
{
    uint32_t line_y = 100;
    uint32_t line_color = 0xFF0000; // Red

    for (uint32_t x = 0; x < framebuffer_width; x++) {
        uint32_t *pixel = (uint32_t *)(framebuffer + line_y * framebuffer_pitch + x * (framebuffer_bpp / 8));
        *pixel = line_color;
    }

    serial_printstr("Line drawn\n");
}

/* Kernel entry point */
void kernel_main(uint64_t magic, void *mbi)
{
    serial_init();
    putchar('1');
    // serial_printstr("Kernel loading\nMagic: ");
    // serial_print_hex(magic);

    putchar('2');

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        putchar('3');
        serial_printstr("Bootloader error: magic isn't valid.\n");
        return;
    }

    /* Assuming Multiboot2, we would parse mbi here for framebuffer info */
    /* Placeholder framebuffer settings for demonstration */
    framebuffer = (uint8_t *)0xE0000000; // Example address (adjust as needed)
    framebuffer_pitch = 1024 * 4;        // 1024 pixels width, 4 bytes per pixel
    framebuffer_width = 1024;
    framebuffer_height = 768;
    framebuffer_bpp = 32;

    // clear_screen(0x0000FF); // Blue
    // serial_printstr("Screen cleared\n");

    // draw_line();
}
