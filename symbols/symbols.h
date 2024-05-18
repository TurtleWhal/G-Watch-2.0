#include "lvgl.h"

LV_FONT_DECLARE(FontAwesome_14);
LV_FONT_DECLARE(FontAwesome_20);
LV_FONT_DECLARE(FontAwesome_32);
LV_FONT_DECLARE(FontAwesome_40);

#define SET_SYMBOL_14(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_14, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_20(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_20, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_32(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_32, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_40(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_40, LV_PART_MAIN); lv_label_set_text(obj, sym);

#define FA_PLAY "\uf04b" // Sizes: [40]
#define FA_PAUSE "\uf04c" // Sizes: [40]
#define FA_STEP_FORWARD "\uf051" // Sizes: [32]
#define FA_STEP_BACKWARD "\uf048" // Sizes: [32]
#define FA_CHEVRON_UP "\uf077" // Sizes: [20]
#define FA_OK "\uf00c" // Sizes: [20]
#define FA_CLOSE "\uf00d" // Sizes: [20]
#define FA_BACKSPACE "\uf55a" // Sizes: [20]
#define FA_BATTERY_FULL "\uf240" // Sizes: [14]
#define FA_BATTERY_3 "\uf241" // Sizes: [14]
#define FA_BATTERY_2 "\uf242" // Sizes: [14]
#define FA_BATTERY_1 "\uf243" // Sizes: [14]
#define FA_BATTERY_EMPTY "\uf244" // Sizes: [14]
#define FA_STEPS "\uf54b" // Sizes: [14]
