/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** string.h
*/

#ifndef STRING_H_
    #define STRING_H_
    #include <stdint.h>
    #include <stddef.h>
    #include <stdarg.h>
    #include "config.h"
    #include "font.h"

void putchar(uint32_t unicode_char);
void puts(const char *str);
void put_uint(uint32_t value);
void put_uint_hex(uint32_t value);
void printf(const char *format, ...);

#endif /* !STRING_H_ */
