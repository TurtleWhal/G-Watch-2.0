/*******************************************************************************
 * Size: 20 px
 * Bpp: 2
 * Opts: --size 20 --bpp 2 --format lvgl --font /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/FontAwesome5.woff --range 0xf077, 0xf00c, 0xf00d, 0xf55a --output /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/generated/FontAwesome_20.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONTAWESOME_20
#define FONTAWESOME_20 1
#endif

#if FONTAWESOME_20

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F00C "" */
    0xf, 0xfe, 0x23, 0xa1, 0xff, 0xc4, 0x62, 0x43,
    0xff, 0x84, 0xc1, 0x41, 0xff, 0xc1, 0x60, 0xd0,
    0x7f, 0xf0, 0x18, 0x34, 0x8e, 0x87, 0xcc, 0x1a,
    0x46, 0x24, 0x39, 0x83, 0x48, 0x20, 0xa4, 0x26,
    0xd, 0x21, 0x41, 0xa4, 0x60, 0xd2, 0x19, 0x83,
    0x54, 0x1a, 0x43, 0xcc, 0x19, 0xd, 0x21, 0xf9,
    0x83, 0xe9, 0xf, 0xf3, 0x7, 0x48, 0x7f, 0xf0,
    0x18, 0x29, 0xf, 0xfe, 0x13, 0x12, 0x1f, 0xe0,

    /* U+F00D "" */
    0x14, 0x1f, 0xa4, 0xac, 0x1e, 0xd9, 0x40, 0x60,
    0xd8, 0x5c, 0x13, 0x3, 0xd, 0xa8, 0x26, 0xc3,
    0x49, 0x21, 0x21, 0xa4, 0x29, 0xf, 0x48, 0x74,
    0x1e, 0x43, 0xd8, 0x7a, 0xe, 0xc3, 0xe6, 0xb,
    0xd, 0x21, 0x31, 0x86, 0x99, 0x9, 0x83, 0x48,
    0x24, 0x36, 0x9, 0xd, 0x20, 0x8f, 0x21, 0xeb,
    0xc0,

    /* U+F077 "" */
    0xf, 0xd2, 0x1f, 0xfc, 0x3d, 0x90, 0xff, 0xe0,
    0xe1, 0x48, 0x7f, 0xd8, 0x74, 0x87, 0xf6, 0x12,
    0x14, 0x87, 0xd8, 0x4d, 0x85, 0x21, 0xd8, 0x4c,
    0xc, 0x29, 0xb, 0x9, 0x83, 0x61, 0x49, 0x4,
    0xc1, 0xec, 0x28, 0xc0, 0xc1, 0xfb, 0xc, 0xd4,
    0x1f, 0xed, 0xc0,

    /* U+F55A "" */
    0xf, 0x3f, 0xff, 0xc4, 0x83, 0xd4, 0xf, 0xfe,
    0x23, 0x6, 0x90, 0xff, 0xe3, 0x21, 0x48, 0x79,
    0x83, 0x48, 0x7e, 0x90, 0xf3, 0x30, 0x26, 0x43,
    0xd2, 0x1f, 0x28, 0x68, 0x8a, 0xe, 0x90, 0xfe,
    0x91, 0x43, 0x7, 0x90, 0xff, 0xa0, 0xd0, 0x7c,
    0x87, 0xfd, 0x6, 0x83, 0xe9, 0xf, 0xe9, 0x14,
    0x30, 0x7d, 0x21, 0xf2, 0x86, 0x88, 0xa0, 0xfa,
    0x43, 0xcc, 0xc0, 0x99, 0xf, 0xd2, 0x1e, 0x60,
    0xd2, 0x1f, 0xe9, 0xf, 0xfe, 0x32, 0x1d, 0x40,
    0xff, 0xe2, 0xc0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 320, .box_w = 20, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 64, .adv_w = 220, .box_w = 14, .box_h = 15, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 280, .box_w = 18, .box_h = 11, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 156, .adv_w = 400, .box_w = 25, .box_h = 15, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x1, 0x6b, 0x54e
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61452, .range_length = 1359, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LV_VERSION_CHECK(8, 0, 0)
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
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
    .bitmap_format = 1,
#if LV_VERSION_CHECK(8, 0, 0)
    .cache = &cache
#endif
};


/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LV_VERSION_CHECK(8, 0, 0)
const lv_font_t FontAwesome_20 = {
#else
lv_font_t FontAwesome_20 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 15,          /*The maximum line height required by the font*/
    .base_line = 0,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -7,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONTAWESOME_20*/

