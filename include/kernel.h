/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** kernel.h
*/

#ifndef KERNEL_H_
    #define KERNEL_H_
    #include <stdint.h>

extern uint8_t *framebuffer;
extern uint32_t framebuffer_pitch; // framebuffer pitch in bytes
extern uint32_t framebuffer_width; // In pixels
extern uint32_t framebuffer_height; // In pixels
extern uint8_t framebuffer_bpp; // Bits per pixel

unsigned int qemu_init_debug();
void qemu_write_string(const char *message);

#endif /* !KERNEL_H_ */
