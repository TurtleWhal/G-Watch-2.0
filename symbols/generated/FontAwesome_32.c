/*******************************************************************************
 * Size: 32 px
 * Bpp: 2
 * Opts: --size 32 --bpp 2 --format lvgl --font /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/FontAwesome5.woff --range 0xf051, 0xf048 --output /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/generated/FontAwesome_32.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONTAWESOME_32
#define FONTAWESOME_32 1
#endif

#if FONTAWESOME_32

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F048 "" */
    0xf, 0xfe, 0x47, 0xe4, 0x3f, 0xe7, 0xc8, 0x6c,
    0x3f, 0xcc, 0x8, 0x3f, 0xf8, 0x4c, 0x1f, 0xfc,
    0x5a, 0x7, 0xff, 0x16, 0x43, 0xff, 0x8b, 0x21,
    0xff, 0xc5, 0xd0, 0xff, 0xe2, 0xe1, 0xff, 0xc6,
    0xc3, 0xff, 0x8d, 0x87, 0xff, 0x1b, 0xf, 0xfe,
    0x36, 0x1f, 0xfc, 0x64, 0x3f, 0xff, 0xe1, 0xff,
    0xcc, 0x60, 0xff, 0xe3, 0xb8, 0x7f, 0xf2, 0x30,
    0xff, 0xe4, 0x61, 0xff, 0xc8, 0xc3, 0xff, 0x91,
    0xa1, 0xff, 0xc7, 0x90, 0xff, 0xe3, 0xc8, 0x7f,
    0xf1, 0xe4, 0x3f, 0xf8, 0xf4, 0xf, 0xd8, 0x7f,
    0x98, 0x17, 0xe4, 0x3f, 0xe7, 0xc8,

    /* U+F051 "" */
    0xf, 0xfe, 0x43, 0xe4, 0x3f, 0xe7, 0xf4, 0x9,
    0xf, 0xf6, 0x1f, 0xa4, 0x3f, 0xf8, 0xf4, 0xf,
    0xfe, 0x3b, 0x7, 0xff, 0x1d, 0x83, 0xff, 0x8e,
    0xe1, 0xff, 0xc8, 0xc3, 0xff, 0x91, 0x87, 0xff,
    0x23, 0xf, 0xfe, 0x46, 0x87, 0xff, 0x1e, 0x43,
    0xff, 0xfe, 0x1f, 0xfc, 0xc4, 0x3f, 0xf8, 0xd8,
    0x7f, 0xf1, 0xb0, 0xff, 0xe3, 0x61, 0xff, 0xc6,
    0xc3, 0xff, 0x8d, 0x87, 0xff, 0x15, 0xc3, 0xff,
    0x8a, 0xc1, 0xff, 0xc5, 0x60, 0xff, 0xe2, 0xd0,
    0x3f, 0xf8, 0xb2, 0x1f, 0xfc, 0x28, 0x12, 0x1f,
    0xec, 0x33, 0xe4, 0x3f, 0xe7, 0xf0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 448, .box_w = 20, .box_h = 30, .ofs_x = 4, .ofs_y = -3},
    {.bitmap_index = 86, .adv_w = 448, .box_w = 20, .box_h = 30, .ofs_x = 4, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x9
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61512, .range_length = 10, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
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
const lv_font_t FontAwesome_32 = {
#else
lv_font_t FontAwesome_32 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 30,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -11,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONTAWESOME_32*/
