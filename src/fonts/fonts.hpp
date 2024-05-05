#include "lvgl.h"

LV_FONT_DECLARE(Outfit_8);
LV_FONT_DECLARE(Outfit_16);
LV_FONT_DECLARE(Outfit_20);
LV_FONT_DECLARE(Outfit_32);
LV_FONT_DECLARE(Outfit_80_Light);

LV_FONT_DECLARE(FontAwesome_20);
LV_FONT_DECLARE(FontAwesome_40);

#define SET_SYMBOL_20(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_20, 0); lv_label_set_text(obj, sym);
#define SET_SYMBOL_40(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_40, 0); lv_label_set_text(obj, sym);

#define FA_PLAY "\xEF\x81\x8B"
#define FA_PAUSE "\xEF\x81\x8C"
#define FA_STOP "\xEF\x81\x8D"
#define FA_REPEAT "\xEF\x81\x8E"
#define FA_STEP_BACKWARD "\xEF\x81\x88"
#define FA_STEP_FORWARD "\xEF\x81\x91"
