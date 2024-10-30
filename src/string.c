#include "../include/string.h"
#include "../include/kernel.h"

uint32_t cx, cy = 0;
psf_font_t *font = (psf_font_t *)&_binary_zap_ext_light32_psf_start;

void putchar(uint32_t unicode_char)
{
    if (unicode_char == '\n') {
        cx = 0; ++cy;
        return;
    }
    __os_putchar(unicode_char, cx, cy, TEXT_COLOR, BACKGROUND_COLOR, font, (uint8_t *)&_binary_zap_ext_light32_psf_start);
    if (++cx > (uint32_t)framebuffer_pitch / font->bytesperglyph) {
        cx = 0; ++cy;
    }
}

void put_uint(uint32_t value)
{
    char buffer[11];
    int i = 10;
    buffer[i] = '\0';

    if (value == 0) {
        putchar('0');
        return;
    }

    while (value > 0 && i > 0) {
        buffer[--i] = '0' + (value % 10);
        value /= 10;
    }

    puts(&buffer[i]);
}


void put_uint_hex(uint32_t value)
{
    char buffer[9];
    const char *hex_chars = "0123456789abcdef";
    int i = 8;
    buffer[i] = '\0';

    if (value == 0) {
        puts("0x0");
        return;
    }

    while (value > 0 && i > 0) {
        buffer[--i] = hex_chars[value & 0xF];
        value >>= 4;
    }

    puts("0x");
    puts(&buffer[i]);
}

void printf(const char *format, ...)
{
    va_list params; va_start(params, format);
    for (uint32_t i = 0; format[i] != '\0'; ++i) {
        if (!(format[i] == '%')) {
            putchar(format[i]);
            continue;
        }
        switch (format[++i]) {
        case 'd': // idc for now
        case 'u':
            put_uint((uint32_t)va_arg(params, uint32_t));
            break;
        case 'x':
            put_uint_hex((uint32_t)va_arg(params, uint32_t));
            break;
        case 'c':
            putchar((uint8_t)va_arg(params, uint32_t));
            break;
        case 's':
            puts((char *)va_arg(params, char *));
            break;
        default:
            putchar(format[i]);
            break;
        }
    }
    va_end(params);
}

void puts(const char *str)
{
    for (uint32_t i = 0; str[i] != '\0'; ++i) putchar(str[i]);
}
