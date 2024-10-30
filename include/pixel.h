/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** pixel.h
*/

#ifndef PIXEL_H_
    #define PIXEL_H_
    #include <stdint.h>

typedef uint32_t argb_pixel_t;

void put_pixel(uint32_t x, uint32_t y, argb_pixel_t color);
void clear_screen(uint32_t color);

#endif /* !PIXEL_H_ */
