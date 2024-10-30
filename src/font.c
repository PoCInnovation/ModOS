#include "../include/font.h"
#include "../include/kernel.h"
#include "../include/pixel.h"
#include "stddef.h"

uint16_t *unicode;

// NOTE: Optional unicode decoding
// Needs calloc which we don't have right now.
//
// void psf_init()
// {
//     uint16_t glyph = 0;
//     psf_font_t *font = (psf_font_t*)&font_start;
//
//     if (font->flags) {
//         unicode = NULL;
//         return;
//     }
//
//     char *s = (char *)(
//     (unsigned char*)&font_start +
//       font->headersize +
//       font->numglyph * font->bytesperglyph
//     );
//     unicode = calloc(USHRT_MAX, 2);
//     while(s>_binary_zap_ext_light32_psf_end) {
//         uint16_t uc = (uint16_t)((unsigned char *)s[0]);
//         if(uc == 0xFF) {
//             glyph++;
//             s++;
//             continue;
//         } else if(uc & 128) {
//             /* UTF-8 to unicode */
//             if((uc & 32) == 0 ) {
//                 uc = ((s[0] & 0x1F)<<6)+(s[1] & 0x3F);
//                 s++;
//             } else
//             if((uc & 16) == 0 ) {
//                 uc = ((((s[0] & 0xF)<<6)+(s[1] & 0x3F))<<6)+(s[2] & 0x3F);
//                 s+=2;
//             } else
//             if((uc & 8) == 0 ) {
//                 uc = ((((((s[0] & 0x7)<<6)+(s[1] & 0x3F))<<6)+(s[2] &
//                 0x3F))<<6)+(s[3] & 0x3F); s+=3;
//             } else
//                 uc = 0;
//         }
//         unicode[uc] = glyph;
//         s++;
//     }
// }

/**
 * @brief Unicode putchar
 *
 * @param c This is an int, not char as it's a unicode character
 * @param cx Horizontal position on screen, in characters not in pixels
 * @param cy Vertical position on screen, in characters not in pixels
 * @param fg ARGB foreground color
 * @param bg ARGB background color
 */
void __os_putchar(
    unsigned short int c,
    int cx, int cy,
    uint32_t fg, uint32_t bg,
    psf_font_t *font,
    uint8_t *font_start)
{
    uint32_t bytesperline = (font->width + 7) / 8;

    if (unicode != NULL) {
        c = unicode[c];
    }

    uint8_t *glyph =
        (uint8_t *)font_start + font->headersize +
        (c > 0 && c < font->numglyph ? c : 0) * font->bytesperglyph;
    uint32_t offset = (cy * font->height * framebuffer_pitch) +
        (cx * font->width * sizeof(argb_pixel_t));

    for (uint32_t y = 0; y < font->height; y++) {
        uint32_t line = offset;
        uint8_t *glyph_row = glyph + y * bytesperline;
        uint32_t bits_left = font->width;

        for (uint32_t b = 0; b < bytesperline; b++) {
            uint8_t byte = glyph_row[b];

            for (int bit = 7; bit >= 0 && bits_left > 0; bit--) {
                uint32_t color = (byte & (1 << bit)) ? fg : bg;

                *((argb_pixel_t *)(framebuffer + line)) = color;

                line += sizeof(argb_pixel_t);
                bits_left--;
            }
        }
        offset += framebuffer_pitch;
    }
}
