/*
** EPITECH PROJECT, 2024
** ModOS
** File description:
** kernel.c
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../include/debug.h"
#include "../include/kernel.h"
#include "../include/multiboot.h"
#include "../include/pixel.h"
#include "../include/config.h"
#include "../include/string.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__x86_64__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

uint8_t *framebuffer;
uint32_t framebuffer_pitch;
uint32_t framebuffer_width;
uint32_t framebuffer_height;
uint8_t framebuffer_bpp;

// Kernel entry
void kmain(void *addr, long magic) {
    struct multiboot_tag *tag;
    unsigned size;
    serial_init();
    serial_printf("Hello %d-bit, world!\n", (int)(sizeof(void*) * 8));
    serial_printf("Magic: %x\n", magic);
    serial_printf("Addr: %x\n", addr);

    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        serial_printstr("Bootloader error: magic isn't valid.");
        return;
    }

    size = *(unsigned *) addr;
    serial_printf("Announced mbi size 0x%x\n", size);

    for (tag = (struct multiboot_tag *) (addr + 8);
       tag->type != MULTIBOOT_TAG_TYPE_END;
       tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag
                                       + ((tag->size + 7) & ~7)))
    {
        serial_printf ("Tag %x, Size %x\n", tag->type, tag->size);
        switch (tag->type)
        {
            //   case MULTIBOOT_TAG_TYPE_CMDLINE:
            //     printf ("Command line = %s\n",
            //             ((struct multiboot_tag_string *) tag)->string);
            //     break;
            //   case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
            //     printf ("Boot loader name = %s\n",
            //             ((struct multiboot_tag_string *) tag)->string);
            //     break;
            //   case MULTIBOOT_TAG_TYPE_MODULE:
            //     printf ("Module at 0x%x-0x%x. Command line %s\n",
            //             ((struct multiboot_tag_module *) tag)->mod_start,
            //             ((struct multiboot_tag_module *) tag)->mod_end,
            //             ((struct multiboot_tag_module *) tag)->cmdline);
            //     break;
            //   case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO:
            //     printf ("mem_lower = %uKB, mem_upper = %uKB\n",
            //             ((struct multiboot_tag_basic_meminfo *) tag)->mem_lower,
            //             ((struct multiboot_tag_basic_meminfo *) tag)->mem_upper);
            //     break;
            //   case MULTIBOOT_TAG_TYPE_BOOTDEV:
            //     printf ("Boot device 0x%x,%u,%u\n",
            //             ((struct multiboot_tag_bootdev *) tag)->biosdev,
            //             ((struct multiboot_tag_bootdev *) tag)->slice,
            //             ((struct multiboot_tag_bootdev *) tag)->part);
            //     break;
            //   case MULTIBOOT_TAG_TYPE_MMAP:
            //     {
            //       multiboot_memory_map_t *mmap;
            //
            //       printf ("mmap\n");
            //
            //       for (mmap = ((struct multiboot_tag_mmap *) tag)->entries;
            //            (multiboot_uint8_t *) mmap
            //              < (multiboot_uint8_t *) tag + tag->size;
            //            mmap = (multiboot_memory_map_t *)
            //              ((unsigned long) mmap
            //               + ((struct multiboot_tag_mmap *) tag)->entry_size))
            //         printf (" base_addr = 0x%x%x,"
            //                 " length = 0x%x%x, type = 0x%x\n",
            //                 (unsigned) (mmap->addr >> 32),
            //                 (unsigned) (mmap->addr & 0xffffffff),
            //                 (unsigned) (mmap->len >> 32),
            //                 (unsigned) (mmap->len & 0xffffffff),
            //                 (unsigned) mmap->type);
            //     }
            //     break;
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER:
                {
                    multiboot_uint32_t color;
                    unsigned i;
                    struct multiboot_tag_framebuffer *tagfb
                        = (struct multiboot_tag_framebuffer *) tag;
                    void *fb = (void *) (unsigned long) tagfb->common.framebuffer_addr;

                    switch (tagfb->common.framebuffer_type) {
                        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
                            {
                                unsigned best_distance, distance;
                                struct multiboot_color *palette;

                                palette = tagfb->framebuffer_palette;

                                color = 0;
                                best_distance = 4*256*256;

                                for (i = 0; i < tagfb->framebuffer_palette_num_colors; i++)
                                {
                                    distance = (0xff - palette[i].blue)
                                        * (0xff - palette[i].blue)
                                        + palette[i].red * palette[i].red
                                        + palette[i].green * palette[i].green;
                                    if (distance < best_distance)
                                    {
                                        color = i;
                                        best_distance = distance;
                                    }
                                }
                                break;
                            }

                        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
                            color = ((1 << tagfb->framebuffer_blue_mask_size) - 1)
                                << tagfb->framebuffer_blue_field_position;
                            break;

                        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
                            color = '\\' | 0x0100;
                            break;

                        default:
                            color = 0xffffffff;
                            break;
                    }

                    framebuffer        = (uint8_t *)(uintptr_t)tagfb->common.framebuffer_addr;
                    framebuffer_pitch  = tagfb->common.framebuffer_pitch;
                    framebuffer_width  = tagfb->common.framebuffer_width;
                    framebuffer_height = tagfb->common.framebuffer_height;
                    framebuffer_bpp    = tagfb->common.framebuffer_bpp;
                    serial_printf("framebuffer: %x\nframebuffer_pitch: %d\nframebuffer_width: %d\nframebuffer_height: %d\nframebuffer_bpp: %d\n", framebuffer, framebuffer_pitch, framebuffer_width, framebuffer_height, framebuffer_bpp);

                    // for (i = 0; i < tagfb->common.framebuffer_width
                    //         && i < tagfb->common.framebuffer_height; i++)
                    // {
                    //     switch (tagfb->common.framebuffer_bpp)
                    //     {
                    //         case 8:
                    //             {
                    //                 multiboot_uint8_t *pixel = fb
                    //                     + tagfb->common.framebuffer_pitch * i + i;
                    //                 *pixel = color;
                    //             }
                    //             break;
                    //         case 15:
                    //         case 16:
                    //             {
                    //                 multiboot_uint16_t *pixel
                    //                     = fb + tagfb->common.framebuffer_pitch * i + 2 * i;
                    //                 *pixel = color;
                    //             }
                    //             break;
                    //         case 24:
                    //             {
                    //                 multiboot_uint32_t *pixel
                    //                     = fb + tagfb->common.framebuffer_pitch * i + 3 * i;
                    //                 *pixel = (color & 0xffffff) | (*pixel & 0xff000000);
                    //             }
                    //             break;
                    //
                    //         case 32:
                    //             {
                    //                 multiboot_uint32_t *pixel
                    //                     = fb + tagfb->common.framebuffer_pitch * i + 4 * i;
                    //                 *pixel = color;
                    //             }
                    //             break;
                    //     }
                    // }
                    break;
                }
        }
    }

    // clear_screen(BACKGROUND_COLOR);

    // printf("Hello %d\n", 42);
}
