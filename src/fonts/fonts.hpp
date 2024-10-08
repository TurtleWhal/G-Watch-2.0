#include "lvgl.h"

LV_FONT_DECLARE(Outfit_8);
LV_FONT_DECLARE(Outfit_16);
LV_FONT_DECLARE(Outfit_20);
LV_FONT_DECLARE(Outfit_32);
LV_FONT_DECLARE(Outfit_46);
LV_FONT_DECLARE(OutfitLight_80);
LV_FONT_DECLARE(OpenSans_80);

LV_FONT_DECLARE(FontAwesome_14);
LV_FONT_DECLARE(FontAwesome_16);
LV_FONT_DECLARE(FontAwesome_20);
LV_FONT_DECLARE(FontAwesome_32);
LV_FONT_DECLARE(FontAwesome_40);

#define SET_SYMBOL_14(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_14, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_16(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_16, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_20(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_20, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_32(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_32, LV_PART_MAIN); lv_label_set_text(obj, sym);
#define SET_SYMBOL_40(obj, sym) lv_obj_set_style_text_font(obj, &FontAwesome_40, LV_PART_MAIN); lv_label_set_text(obj, sym);

#define FA_PLAY           "\xEF\x81\x8B" // U+F04B, Sizes: [40, 32]
#define FA_PAUSE          "\xEF\x81\x8C" // U+F04C, Sizes: [40, 32]
#define FA_BRIGHTNESS     "\xEE\x83\x89" // U+E0C9, Sizes: [40]
#define FA_BRIGHTNESS_LOW "\xEE\x83\x8A" // U+E0CA, Sizes: [40]
#define FA_FLASHLIGHT     "\xEF\xA2\xB8" // U+F8B8, Sizes: [32]
#define FA_STEP_FORWARD   "\xEF\x81\x91" // U+F051, Sizes: [32]
#define FA_STEP_BACKWARD  "\xEF\x81\x88" // U+F048, Sizes: [32]
#define FA_RESET          "\xEF\x8B\xB1" // U+F2F1, Sizes: [32]
#define FA_ARROW_RIGHT    "\xEF\x81\xA1" // U+F061, Sizes: [32]
#define FA_CHEVRON_UP     "\xEF\x81\xB7" // U+F077, Sizes: [20]
#define FA_OK             "\xEF\x80\x8C" // U+F00C, Sizes: [20]
#define FA_CLOSE          "\xEF\x80\x8D" // U+F00D, Sizes: [20]
#define FA_BACKSPACE      "\xEF\x95\x9A" // U+F55A, Sizes: [20]
#define FA_SEND           "\xEE\x88\x8A" // U+E20A, Sizes: [16]
#define FA_CHARGE         "\xEF\x83\xA7" // U+F0E7, Sizes: [14]
#define FA_BATTERY_FULL   "\xEF\x89\x80" // U+F240, Sizes: [14]
#define FA_BATTERY_3      "\xEF\x89\x81" // U+F241, Sizes: [14]
#define FA_BATTERY_2      "\xEF\x89\x82" // U+F242, Sizes: [14]
#define FA_BATTERY_1      "\xEF\x89\x83" // U+F243, Sizes: [14]
#define FA_BATTERY_EMPTY  "\xEF\x89\x84" // U+F244, Sizes: [14]
#define FA_STEPS          "\xEF\x95\x8B" // U+F54B, Sizes: [14]
#define FA_BLUETOOTH      "\xEF\x8A\x94" // U+F294, Sizes: [14, 32]
#define FA_USB            "\xEF\x8A\x87" // U+F287, Sizes: [14]
#define FA_WIFI           "\xEF\x87\xAB" // U+F1EB, Sizes: [14]
#define FA_ENVELOPE       "\xEF\x83\xA0" // U+F0E0, Sizes: [14]
#define FA_POINT          "\xEE\x84\xA2" // U+E122, Sizes: [14]
#define FA_DO_NOT_DISTURB "\xEF\x81\x96" // U+F056, Sizes: [14, 32]
#define FA_TIMER          "\xEF\x89\x94" // U+F254, Sizes: [16]
#define FA_STOPWATCH      "\xEF\x8B\xB2" // U+F2F2, Sizes: [16]
#define FA_MUSIC_NOTE     "\xEF\xA3\x8F" // U+F8CF, Sizes: [16]
#define FA_BELL           "\xEF\x83\xB3" // U+F0F3, Sizes: [14, 40]
#define FA_DISCORD        "\xEF\x8E\x92" // U+F392, Sizes: [14, 40]
#define FA_MESSAGE        "\xEF\x81\xB5" // U+F075, Sizes: [14, 40]
#define FA_SMS            "\xEF\x9F\x8D" // U+F7CD, Sizes: [14, 40]
#define FA_EMAIL          "\xEF\x83\xA0" // U+F0E0, Sizes: [14, 40]
#define FA_GADGETBRIDGE   "\xEF\x8B\xA1" // U+F2E1, Sizes: [14, 40]
#define FA_YOUTUBE        "\xEF\x85\xA7" // U+F167, Sizes: [14, 40]
