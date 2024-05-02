/*******************************************************************************
 * Size: 40 px
 * Bpp: 2
 * Opts: --bpp 2 --size 40 --no-compress --font Lato-Bold.ttf --symbols 1234567890: --format lvgl -o Lato_40_bold.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef LATO_40_BOLD
#define LATO_40_BOLD 1
#endif

#if LATO_40_BOLD

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0030 "0" */
    0x0, 0x1, 0xff, 0xe0, 0x0, 0x0, 0x2, 0xff,
    0xff, 0xe0, 0x0, 0x0, 0xff, 0xff, 0xff, 0xc0,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0x0, 0xb, 0xff,
    0x91, 0xbf, 0xfc, 0x0, 0xff, 0xd0, 0x1, 0xff,
    0xd0, 0x2f, 0xf8, 0x0, 0x7, 0xff, 0x3, 0xff,
    0x0, 0x0, 0x3f, 0xf0, 0x7f, 0xf0, 0x0, 0x2,
    0xff, 0x8b, 0xfe, 0x0, 0x0, 0x1f, 0xf8, 0xbf,
    0xd0, 0x0, 0x0, 0xff, 0xcf, 0xfc, 0x0, 0x0,
    0xf, 0xfc, 0xff, 0xc0, 0x0, 0x0, 0xff, 0xcf,
    0xfc, 0x0, 0x0, 0xf, 0xfc, 0xff, 0xc0, 0x0,
    0x0, 0xff, 0xcf, 0xfc, 0x0, 0x0, 0xf, 0xfc,
    0xff, 0xc0, 0x0, 0x0, 0xff, 0xcf, 0xfc, 0x0,
    0x0, 0xf, 0xfc, 0xbf, 0xd0, 0x0, 0x0, 0xff,
    0xc7, 0xfe, 0x0, 0x0, 0x1f, 0xf8, 0x7f, 0xf0,
    0x0, 0x2, 0xff, 0x43, 0xff, 0x0, 0x0, 0x3f,
    0xf0, 0x2f, 0xf8, 0x0, 0x7, 0xff, 0x0, 0xff,
    0xd0, 0x1, 0xff, 0xd0, 0xb, 0xff, 0x91, 0xbf,
    0xf8, 0x0, 0x3f, 0xff, 0xff, 0xff, 0x0, 0x0,
    0xff, 0xff, 0xff, 0xc0, 0x0, 0x2, 0xff, 0xff,
    0xe0, 0x0, 0x0, 0x1, 0xff, 0xe0, 0x0, 0x0,

    /* U+0031 "1" */
    0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0xb, 0xff,
    0x0, 0x0, 0x0, 0xbf, 0xfc, 0x0, 0x0, 0xf,
    0xff, 0xf0, 0x0, 0x0, 0xff, 0xff, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0x0, 0x2, 0xff, 0xff, 0xfc,
    0x0, 0x1f, 0xff, 0x2f, 0xf0, 0x0, 0x3f, 0xe0,
    0xff, 0xc0, 0x0, 0x3e, 0x3, 0xff, 0x0, 0x0,
    0x0, 0xf, 0xfc, 0x0, 0x0, 0x0, 0x3f, 0xf0,
    0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x3,
    0xff, 0x0, 0x0, 0x0, 0xf, 0xfc, 0x0, 0x0,
    0x0, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0xff, 0xc0,
    0x0, 0x0, 0x3, 0xff, 0x0, 0x0, 0x0, 0xf,
    0xfc, 0x0, 0x0, 0x0, 0x3f, 0xf0, 0x0, 0x0,
    0x0, 0xff, 0xc0, 0x0, 0x0, 0x3, 0xff, 0x0,
    0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x0, 0x3f,
    0xf0, 0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x1a,
    0xab, 0xff, 0xea, 0x90, 0xbf, 0xff, 0xff, 0xff,
    0x42, 0xff, 0xff, 0xff, 0xfd, 0xb, 0xff, 0xff,
    0xff, 0xf4,

    /* U+0032 "2" */
    0x0, 0x1, 0xbf, 0xe4, 0x0, 0x0, 0xb, 0xff,
    0xff, 0xe0, 0x0, 0xf, 0xff, 0xff, 0xfe, 0x0,
    0xf, 0xff, 0xff, 0xff, 0xe0, 0xb, 0xff, 0x91,
    0x7f, 0xfc, 0x3, 0xff, 0x40, 0x3, 0xff, 0x81,
    0xff, 0x80, 0x0, 0x7f, 0xf0, 0xbf, 0xc0, 0x0,
    0xf, 0xfc, 0x1a, 0xd0, 0x0, 0x3, 0xff, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x7f, 0xe0, 0x0, 0x0, 0x0, 0x2f, 0xf4, 0x0,
    0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x0, 0xf,
    0xfd, 0x0, 0x0, 0x0, 0xb, 0xfe, 0x0, 0x0,
    0x0, 0xb, 0xff, 0x0, 0x0, 0x0, 0xb, 0xff,
    0x0, 0x0, 0x0, 0xb, 0xff, 0x0, 0x0, 0x0,
    0xb, 0xff, 0x0, 0x0, 0x0, 0xb, 0xff, 0x0,
    0x0, 0x0, 0xb, 0xff, 0x0, 0x0, 0x0, 0xb,
    0xff, 0x0, 0x0, 0x0, 0xb, 0xff, 0x0, 0x0,
    0x0, 0xb, 0xff, 0x0, 0x0, 0x0, 0xb, 0xff,
    0x5a, 0xaa, 0xa9, 0xb, 0xff, 0xff, 0xff, 0xff,
    0xd3, 0xff, 0xff, 0xff, 0xff, 0xf9, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0xff, 0xff,
    0x80,

    /* U+0033 "3" */
    0x0, 0x1, 0xbf, 0xf9, 0x0, 0x0, 0x7, 0xff,
    0xff, 0xf0, 0x0, 0xb, 0xff, 0xff, 0xff, 0x40,
    0x7, 0xff, 0xff, 0xff, 0xf0, 0x3, 0xff, 0xd4,
    0x6f, 0xfe, 0x2, 0xff, 0xc0, 0x1, 0xff, 0xc0,
    0xff, 0xc0, 0x0, 0x3f, 0xf0, 0x3f, 0xe0, 0x0,
    0xb, 0xfc, 0x1, 0x50, 0x0, 0x2, 0xff, 0x0,
    0x0, 0x0, 0x0, 0xff, 0xc0, 0x0, 0x0, 0x0,
    0x7f, 0xd0, 0x0, 0x0, 0x1, 0xff, 0xe0, 0x0,
    0x0, 0x3f, 0xff, 0xd0, 0x0, 0x0, 0xf, 0xff,
    0xd0, 0x0, 0x0, 0x3, 0xff, 0xff, 0x0, 0x0,
    0x0, 0x6b, 0xff, 0xf4, 0x0, 0x0, 0x0, 0xb,
    0xff, 0x0, 0x0, 0x0, 0x0, 0xbf, 0xd0, 0x0,
    0x0, 0x0, 0x1f, 0xf8, 0x0, 0x0, 0x0, 0x3,
    0xff, 0x6, 0xc0, 0x0, 0x0, 0xff, 0xcf, 0xfc,
    0x0, 0x0, 0x7f, 0xe2, 0xff, 0x80, 0x0, 0x3f,
    0xf4, 0x7f, 0xf4, 0x0, 0x2f, 0xfc, 0xf, 0xff,
    0xd1, 0x7f, 0xfe, 0x0, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x1f, 0xff, 0xff, 0xff, 0x0, 0x0, 0xff,
    0xff, 0xfe, 0x0, 0x0, 0x6, 0xff, 0xe4, 0x0,
    0x0,

    /* U+0034 "4" */
    0x0, 0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x0,
    0x0, 0xff, 0xf0, 0x0, 0x0, 0x0, 0xb, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0x7f, 0xff, 0x0, 0x0,
    0x0, 0x3, 0xff, 0xfc, 0x0, 0x0, 0x0, 0x3f,
    0xff, 0xf0, 0x0, 0x0, 0x2, 0xff, 0xbf, 0xc0,
    0x0, 0x0, 0x1f, 0xf5, 0xff, 0x0, 0x0, 0x0,
    0xff, 0x87, 0xfc, 0x0, 0x0, 0xf, 0xfc, 0x1f,
    0xf0, 0x0, 0x0, 0xbf, 0xc0, 0x7f, 0xc0, 0x0,
    0x7, 0xfd, 0x1, 0xff, 0x0, 0x0, 0x3f, 0xe0,
    0x7, 0xfc, 0x0, 0x3, 0xff, 0x0, 0x1f, 0xf0,
    0x0, 0x2f, 0xf4, 0x0, 0x7f, 0xc0, 0x1, 0xff,
    0x80, 0x1, 0xff, 0x0, 0xf, 0xfc, 0x0, 0x7,
    0xfc, 0x0, 0xff, 0xc0, 0x0, 0x1f, 0xf0, 0x7,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x5f, 0xff, 0xff,
    0xff, 0xff, 0xfd, 0x3f, 0xff, 0xff, 0xff, 0xff,
    0xf4, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x0,
    0x0, 0x0, 0x7f, 0xc0, 0x0, 0x0, 0x0, 0x1,
    0xff, 0x0, 0x0, 0x0, 0x0, 0x7, 0xfc, 0x0,
    0x0, 0x0, 0x0, 0x1f, 0xf0, 0x0, 0x0, 0x0,
    0x0, 0x7f, 0xc0, 0x0, 0x0, 0x0, 0x1, 0xff,
    0x0, 0x0, 0x0, 0x0, 0x7, 0xfc, 0x0,

    /* U+0035 "5" */
    0x0, 0xbf, 0xff, 0xff, 0xfc, 0x0, 0xff, 0xff,
    0xff, 0xfc, 0x0, 0xff, 0xff, 0xff, 0xf8, 0x0,
    0xff, 0xff, 0xff, 0xf4, 0x1, 0xff, 0xaa, 0xaa,
    0x40, 0x1, 0xfe, 0x0, 0x0, 0x0, 0x2, 0xfe,
    0x0, 0x0, 0x0, 0x3, 0xfd, 0x0, 0x0, 0x0,
    0x3, 0xfc, 0x0, 0x0, 0x0, 0x3, 0xfc, 0x0,
    0x0, 0x0, 0x7, 0xff, 0xff, 0xe4, 0x0, 0x7,
    0xff, 0xff, 0xff, 0x0, 0xb, 0xff, 0xff, 0xff,
    0xd0, 0xf, 0xff, 0xff, 0xff, 0xf0, 0x1, 0xa4,
    0x1, 0xbf, 0xfc, 0x0, 0x0, 0x0, 0x1f, 0xfc,
    0x0, 0x0, 0x0, 0xb, 0xfd, 0x0, 0x0, 0x0,
    0x7, 0xfe, 0x0, 0x0, 0x0, 0x3, 0xfe, 0x0,
    0x0, 0x0, 0x3, 0xfe, 0x0, 0x0, 0x0, 0x7,
    0xfe, 0x0, 0x0, 0x0, 0xb, 0xfd, 0x0, 0x0,
    0x0, 0xf, 0xfc, 0xf, 0xc0, 0x0, 0x3f, 0xf8,
    0x2f, 0xf9, 0x6, 0xff, 0xf0, 0x7f, 0xff, 0xff,
    0xff, 0xd0, 0x1f, 0xff, 0xff, 0xff, 0x40, 0x2,
    0xff, 0xff, 0xf8, 0x0, 0x0, 0x1b, 0xfe, 0x40,
    0x0,

    /* U+0036 "6" */
    0x0, 0x0, 0x1, 0xff, 0xe0, 0x0, 0x0, 0x0,
    0xff, 0xf0, 0x0, 0x0, 0x0, 0xff, 0xf0, 0x0,
    0x0, 0x0, 0xbf, 0xf0, 0x0, 0x0, 0x0, 0x7f,
    0xf0, 0x0, 0x0, 0x0, 0x3f, 0xf4, 0x0, 0x0,
    0x0, 0x3f, 0xf4, 0x0, 0x0, 0x0, 0x2f, 0xf8,
    0x0, 0x0, 0x0, 0x1f, 0xf8, 0x0, 0x0, 0x0,
    0xf, 0xfc, 0x0, 0x0, 0x0, 0xf, 0xfd, 0xaa,
    0x40, 0x0, 0xb, 0xff, 0xff, 0xfe, 0x0, 0x7,
    0xff, 0xff, 0xff, 0xf4, 0x3, 0xff, 0xff, 0xff,
    0xff, 0x1, 0xff, 0xf4, 0x6, 0xff, 0xf0, 0xbf,
    0xf0, 0x0, 0x1f, 0xfd, 0x3f, 0xf0, 0x0, 0x2,
    0xff, 0x8f, 0xfc, 0x0, 0x0, 0x7f, 0xf3, 0xfe,
    0x0, 0x0, 0xf, 0xfc, 0xff, 0x80, 0x0, 0x3,
    0xff, 0x3f, 0xe0, 0x0, 0x0, 0xff, 0xcf, 0xfc,
    0x0, 0x0, 0x7f, 0xe2, 0xff, 0x0, 0x0, 0x3f,
    0xf4, 0x7f, 0xf0, 0x0, 0x2f, 0xfc, 0xf, 0xff,
    0x40, 0x7f, 0xfd, 0x0, 0xff, 0xff, 0xff, 0xfe,
    0x0, 0x1f, 0xff, 0xff, 0xfe, 0x0, 0x0, 0xbf,
    0xff, 0xfd, 0x0, 0x0, 0x2, 0xff, 0xe4, 0x0,
    0x0,

    /* U+0037 "7" */
    0x3f, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xff, 0xff,
    0xff, 0xff, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xfc,
    0xff, 0xff, 0xff, 0xff, 0xfe, 0x1a, 0xaa, 0xaa,
    0xaa, 0xff, 0x40, 0x0, 0x0, 0x0, 0xff, 0xc0,
    0x0, 0x0, 0x0, 0x7f, 0xd0, 0x0, 0x0, 0x0,
    0x3f, 0xf0, 0x0, 0x0, 0x0, 0x1f, 0xf4, 0x0,
    0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x0, 0xb,
    0xfd, 0x0, 0x0, 0x0, 0x3, 0xff, 0x0, 0x0,
    0x0, 0x2, 0xff, 0x80, 0x0, 0x0, 0x0, 0xff,
    0xc0, 0x0, 0x0, 0x0, 0xbf, 0xe0, 0x0, 0x0,
    0x0, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0x2f, 0xf8,
    0x0, 0x0, 0x0, 0xf, 0xfc, 0x0, 0x0, 0x0,
    0xb, 0xfe, 0x0, 0x0, 0x0, 0x3, 0xff, 0x0,
    0x0, 0x0, 0x2, 0xff, 0x80, 0x0, 0x0, 0x0,
    0xff, 0xc0, 0x0, 0x0, 0x0, 0xbf, 0xe0, 0x0,
    0x0, 0x0, 0x3f, 0xf0, 0x0, 0x0, 0x0, 0x2f,
    0xf8, 0x0, 0x0, 0x0, 0xf, 0xfc, 0x0, 0x0,
    0x0, 0xb, 0xff, 0x0, 0x0, 0x0, 0x3, 0xff,
    0x0, 0x0, 0x0, 0x2, 0xff, 0x40, 0x0, 0x0,
    0x0,

    /* U+0038 "8" */
    0x0, 0x2, 0xff, 0xe4, 0x0, 0x0, 0xf, 0xff,
    0xff, 0xd0, 0x0, 0x1f, 0xff, 0xff, 0xfd, 0x0,
    0xf, 0xff, 0xff, 0xff, 0xd0, 0xb, 0xff, 0x0,
    0x2f, 0xfc, 0x3, 0xff, 0x0, 0x3, 0xff, 0x41,
    0xff, 0x80, 0x0, 0x7f, 0xe0, 0x7f, 0xd0, 0x0,
    0xf, 0xf8, 0x1f, 0xf4, 0x0, 0x7, 0xfe, 0x3,
    0xfe, 0x0, 0x1, 0xff, 0x40, 0xff, 0xc0, 0x0,
    0xff, 0xc0, 0x1f, 0xfd, 0x1, 0xff, 0xe0, 0x1,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0xf, 0xff, 0xff,
    0xd0, 0x0, 0xb, 0xff, 0xff, 0xfc, 0x0, 0x1f,
    0xff, 0xff, 0xff, 0xe0, 0xf, 0xfe, 0x0, 0x2f,
    0xfd, 0xf, 0xfe, 0x0, 0x1, 0xff, 0xc3, 0xff,
    0x0, 0x0, 0x3f, 0xf5, 0xff, 0x80, 0x0, 0xb,
    0xfe, 0x7f, 0xe0, 0x0, 0x1, 0xff, 0x9f, 0xf8,
    0x0, 0x0, 0xbf, 0xe3, 0xff, 0x0, 0x0, 0x3f,
    0xf4, 0xff, 0xe0, 0x0, 0x1f, 0xfc, 0x1f, 0xfe,
    0x0, 0x2f, 0xfe, 0x3, 0xff, 0xff, 0xff, 0xff,
    0x0, 0x3f, 0xff, 0xff, 0xff, 0x0, 0x1, 0xff,
    0xff, 0xfe, 0x0, 0x0, 0x6, 0xff, 0xe4, 0x0,
    0x0,

    /* U+0039 "9" */
    0x0, 0x6, 0xff, 0xe4, 0x0, 0x0, 0xf, 0xff,
    0xff, 0xc0, 0x0, 0x1f, 0xff, 0xff, 0xfd, 0x0,
    0x1f, 0xff, 0xff, 0xff, 0xc0, 0xf, 0xff, 0x40,
    0x7f, 0xfc, 0xb, 0xfe, 0x0, 0x3, 0xff, 0x43,
    0xff, 0x0, 0x0, 0x3f, 0xe1, 0xff, 0x80, 0x0,
    0xf, 0xfc, 0x7f, 0xe0, 0x0, 0x2, 0xff, 0x1f,
    0xf8, 0x0, 0x0, 0xbf, 0xc7, 0xff, 0x0, 0x0,
    0x3f, 0xf0, 0xff, 0xd0, 0x0, 0x2f, 0xfc, 0x2f,
    0xfd, 0x0, 0x2f, 0xfe, 0x3, 0xff, 0xff, 0xff,
    0xff, 0x40, 0x7f, 0xff, 0xff, 0xff, 0xc0, 0x7,
    0xff, 0xff, 0xff, 0xe0, 0x0, 0x1b, 0xfe, 0x7f,
    0xf0, 0x0, 0x0, 0x0, 0x3f, 0xf4, 0x0, 0x0,
    0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0x2f, 0xfc,
    0x0, 0x0, 0x0, 0x1f, 0xfd, 0x0, 0x0, 0x0,
    0xf, 0xfe, 0x0, 0x0, 0x0, 0xf, 0xff, 0x0,
    0x0, 0x0, 0xb, 0xff, 0x40, 0x0, 0x0, 0x7,
    0xff, 0x80, 0x0, 0x0, 0x3, 0xff, 0xc0, 0x0,
    0x0, 0x3, 0xff, 0xc0, 0x0, 0x0, 0x2, 0xff,
    0xd0, 0x0, 0x0, 0x1, 0xff, 0xd0, 0x0, 0x0,
    0x0,

    /* U+003A ":" */
    0x0, 0x0, 0x7f, 0x7, 0xff, 0x2f, 0xfc, 0xbf,
    0xf1, 0xff, 0xc1, 0xf8, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x7, 0xe0, 0x7f, 0xf2, 0xff, 0xcb,
    0xff, 0x1f, 0xfc, 0x1f, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 371, .box_w = 22, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 160, .adv_w = 371, .box_w = 19, .box_h = 29, .ofs_x = 3, .ofs_y = 0},
    {.bitmap_index = 298, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 604, .adv_w = 371, .box_w = 23, .box_h = 29, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 771, .adv_w = 371, .box_w = 20, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 916, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1069, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1222, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1375, .adv_w = 371, .box_w = 21, .box_h = 29, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1528, .adv_w = 170, .box_w = 7, .box_h = 21, .ofs_x = 2, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 48, .range_length = 11, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 1,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t Lato_40_bold = {
#else
lv_font_t Lato_40_bold = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 29,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 3,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if LATO_40_BOLD*/

