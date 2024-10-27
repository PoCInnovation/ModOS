#include <stdint.h>
#include <stdarg.h>
#include <stdarg.h>

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init()
{
    outb(0x3F8 + 1, 0x00);    // Disable all interrupts
    outb(0x3F8 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(0x3F8 + 1, 0x00);    //                  (hi byte)
    outb(0x3F8 + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(0x3F8 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(0x3F8 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
}

int serial_transmit_fifo_empty()
{
    return inb(0x3F8 + 5) & 0x20;
}

void serial_putchar(char c)
{
    while (serial_transmit_fifo_empty() == 0);
    outb(0x3F8, c);
}

void serial_printstr(const char *str)
{
    while (*str) {
        serial_putchar(*str++);
    }
}


void serial_print_dec(uint32_t value)
{
    char buffer[11];
    int i = 10;
    buffer[i] = '\0';

    if (value == 0) {
        serial_putchar('0');
        return;
    }

    while (value > 0 && i > 0) {
        buffer[--i] = '0' + (value % 10);
        value /= 10;
    }

    serial_printstr(&buffer[i]);
}


void serial_print_hex(uint32_t value)
{
    char buffer[9];
    const char *hex_chars = "0123456789abcdef";
    int i = 8;
    buffer[i] = '\0';

    if (value == 0) {
        serial_printstr("0x0");
        return;
    }

    while (value > 0 && i > 0) {
        buffer[--i] = hex_chars[value & 0xF];
        value >>= 4;
    }

    serial_printstr("0x");
    serial_printstr(&buffer[i]);
}

void serial_printf(const char *format, ...)
{
    va_list params; va_start(params, format);
    for (uint32_t i = 0; format[i] != '\0'; ++i) {
        if (!(format[i] == '%')) {
            serial_putchar(format[i]);
            continue;
        }
        switch (format[++i]) {
        case 'd': // idc for now
        case 'u':
            serial_print_dec((uint32_t)va_arg(params, uint32_t));
            break;
        case 'x':
            serial_print_hex((uint32_t)va_arg(params, uint32_t));
            break;
        case 'c':
            serial_putchar((uint8_t)va_arg(params, uint32_t));
            break;
        case 's':
            serial_printstr((char *)va_arg(params, char *));
            break;
        default:
            serial_putchar(format[i]);
            break;
        }
    }
    va_end(params);
}
