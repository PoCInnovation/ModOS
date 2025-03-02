/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** debug.h
*/

#ifndef DEBUG_H_
    #define DEBUG_H_
    #include <stdint.h>

void serial_init();
void serial_printstr(const char *str);
void serial_print_hex(uint32_t value);
void serial_print_dec(uint32_t value);
void serial_printf(const char *format, ...);

#endif /* !DEBUG_H_ */
