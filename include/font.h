/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** font.h
*/

#ifndef FONT_H_
    #define FONT_H_
    #include <stdint.h>
    #define PSF1_FONT_MAGIC 0x0436
    #define PSF_FONT_MAGIC 0x864ab572

typedef struct {
    uint16_t magic;        // Magic bytes for identification.
    uint8_t fontMode;      // PSF font mode.
    uint8_t characterSize; // PSF character size.
} psf1_header_t;

typedef struct {
    uint32_t magic;         /* magic bytes to identify PSF */
    uint32_t version;       /* zero */
    uint32_t headersize;    /* offset of bitmaps in file, 32 */
    uint32_t flags;         /* 0 if there's no unicode table */
    uint32_t numglyph;      /* number of glyphs */
    uint32_t bytesperglyph; /* size of each glyph */
    uint32_t height;        /* height in pixels */
    uint32_t width;         /* width in pixels */
} psf_font_t;

extern uint8_t _binary_zap_ext_light32_psf_start;
extern uint8_t _binary_zap_ext_light32_psf_end;
extern uint8_t _binary_zap_ext_light32_psf_size;

void __os_putchar(unsigned short int c, int cx, int cy, uint32_t fg,
                  uint32_t bg, psf_font_t *font, uint8_t *font_start);
                  // , psf_font_t *font, uint8_t font_start[]);

#endif /* !FONT_H_ */
