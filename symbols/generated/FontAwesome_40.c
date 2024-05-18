/*******************************************************************************
 * Size: 40 px
 * Bpp: 2
 * Opts: --size 40 --bpp 2 --format lvgl --font /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/FontAwesome5.woff --range 0xf04b, 0xf04c --output /Users/gj0987888/Documents/GitHub/G-Watch-2.0/fonts/generated/FontAwesome_40.c
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifndef FONTAWESOME_40
#define FONTAWESOME_40 1
#endif

#if FONTAWESOME_40

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+F04B "" */
    0xf, 0xfe, 0xc3, 0xf4, 0x1f, 0xfd, 0x16, 0x9,
    0xd0, 0xff, 0xe7, 0xc1, 0xeb, 0xf, 0xfe, 0xbf,
    0x7, 0xff, 0x59, 0xd0, 0xff, 0xeb, 0x58, 0x7f,
    0xf5, 0xf8, 0x3f, 0xfa, 0xce, 0x87, 0xff, 0x5a,
    0xd0, 0xff, 0xeb, 0x50, 0x3f, 0xfa, 0xce, 0x87,
    0xff, 0x5a, 0xd0, 0xff, 0xeb, 0x50, 0x3f, 0xfa,
    0xcf, 0x7, 0xff, 0x59, 0xd0, 0xff, 0xeb, 0x58,
    0x7f, 0xf5, 0xf8, 0x3f, 0xfa, 0xce, 0x1f, 0xfd,
    0x78, 0x3f, 0xfa, 0xc8, 0x7f, 0xfe, 0x50, 0xff,
    0xeb, 0x41, 0xff, 0xd4, 0x70, 0xff, 0xea, 0x70,
    0x7f, 0xf4, 0xec, 0x3f, 0xfa, 0x6e, 0x87, 0xff,
    0x49, 0xe0, 0xff, 0xe9, 0xd0, 0x3f, 0xfa, 0x76,
    0x87, 0xff, 0x49, 0xd0, 0xff, 0xe9, 0xd0, 0x3f,
    0xfa, 0x76, 0x87, 0xff, 0x49, 0xd0, 0xff, 0xe9,
    0xf0, 0x7f, 0xf4, 0xec, 0x3f, 0xfa, 0x6e, 0x87,
    0xff, 0x4f, 0x83, 0xff, 0x99, 0x7, 0xac, 0x3f,
    0xf9, 0xce, 0x81, 0xd0, 0xff, 0xe8, 0xde, 0x83,
    0xff, 0xa2,

    /* U+F04C "" */
    0x5, 0xff, 0xd0, 0x7f, 0x5f, 0xfd, 0x5, 0xa1,
    0xfc, 0xe1, 0xf6, 0x87, 0xf3, 0x90, 0x7f, 0xf0,
    0x60, 0xe8, 0x3f, 0xf8, 0x32, 0x1f, 0xfc, 0x14,
    0x39, 0xf, 0xfe, 0xa, 0x1f, 0xff, 0xf0, 0xff,
    0xff, 0x87, 0xff, 0xfc, 0x3f, 0xff, 0xe1, 0xff,
    0xff, 0xf, 0xff, 0xf8, 0x7f, 0xff, 0xc3, 0xff,
    0xfe, 0x1f, 0xff, 0xf0, 0xff, 0xff, 0x87, 0xff,
    0xfc, 0x3f, 0xff, 0xe1, 0xff, 0xff, 0xf, 0xfe,
    0xe4, 0x1f, 0xfc, 0x18, 0x3a, 0xf, 0xfe, 0xc,
    0xc1, 0xff, 0x48, 0x73, 0x7, 0xfd, 0x23, 0xff,
    0xf2, 0x1f, 0x3f, 0xff, 0x20
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 560, .box_w = 35, .box_h = 42, .ofs_x = 0, .ofs_y = -6},
    {.bitmap_index = 138, .adv_w = 560, .box_w = 35, .box_h = 36, .ofs_x = 0, .ofs_y = -3}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 61515, .range_length = 2, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
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
const lv_font_t FontAwesome_40 = {
#else
lv_font_t FontAwesome_40 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 42,          /*The maximum line height required by the font*/
    .base_line = 6,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -14,
    .underline_thickness = 2,
#endif
    .dsc = &font_dsc           /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
};



#endif /*#if FONTAWESOME_40*/

