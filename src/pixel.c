#include "../include/kernel.h"
#include "../include/pixel.h"
#include "../include/debug.h"

/**
 * @return the byte offset for the give (x, y) pixel position
 */
static inline uint32_t position(uint32_t x, uint32_t y)
{
    return (y * framebuffer_pitch) + (x * (framebuffer_bpp) / 8);
}

/**
 * x and y indicate the pixel position
 * color is a 32 bit unsigned integer such as 0xAARRGGBB
 */
void put_pixel(uint32_t x, uint32_t y, argb_pixel_t color)
{
    const uint32_t index = position(x, y);

    if ((index + (framebuffer_bpp / 8)) > (framebuffer_height * framebuffer_pitch)) {
        serial_printf("Out of bounds put_pixel (x=%d, y=%d)\n", x, y);
        return;
    }

    switch (framebuffer_bpp) {
    case 32:
        framebuffer[index + 3] = (color & 0xff000000) >> 24; // FIX: crash
        framebuffer[index + 2] = (color & 0x00ff0000) >> 16;
        framebuffer[index + 1] = (color & 0x0000ff00) >> 8;
        framebuffer[index]     = (color & 0x000000ff);
        break;
    case 24:
        framebuffer[index + 2] = (color & 0xff0000) >> 16;
        framebuffer[index + 1] = (color & 0x00ff00) >> 8;
        framebuffer[index]     = (color & 0x0000ff);
        break;
    case 16:
        framebuffer[index + 1] = (color & 0xff00) >> 8;
        framebuffer[index]     = (color & 0x00ff);
        break;
    }
}

void clear_screen(argb_pixel_t color)
{
    for (uint32_t y = 0; y < framebuffer_height; ++y) {
        for (uint32_t x = 0; x < framebuffer_width; ++x) {
            put_pixel(x, y, color);
        }
    }
}
