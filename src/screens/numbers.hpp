#include "Arduino.h"
#include "lvgl.h"
#include "system.hpp"

LV_FONT_DECLARE(Jakarta_100_medium);

class NumbersScreen
{
private:
    lv_obj_t *hour;
    lv_obj_t *minute;

    lv_obj_t *batnum;
    lv_obj_t *batarc;
    lv_obj_t *baticn;

public:
    void create()
    {
        scr = lv_obj_create(nullptr);

        hour = lv_label_create(scr);
        minute = lv_label_create(scr);

        lv_label_set_text(hour, "00");
        lv_label_set_text(minute, "00");

        lv_obj_align(hour, LV_ALIGN_CENTER, 30, -40);
        lv_obj_align(minute, LV_ALIGN_CENTER, 30, 40);

        lv_obj_set_style_text_font(hour, &Jakarta_100_medium, 0);
        lv_obj_set_style_text_font(minute, &Jakarta_100_medium, 0);

        lv_obj_set_style_text_color(hour, lv_color_lighten(lv_theme_get_color_primary(scr), 100), 0);
        lv_obj_set_style_text_color(minute, lv_color_lighten(lv_theme_get_color_primary(scr), 200), 0);

        int32_t batx = -75;

        batnum = lv_label_create(scr);
        lv_obj_align(batnum, LV_ALIGN_CENTER, batx, 0);

        batarc = lv_arc_create(scr);
        lv_obj_align(batarc, LV_ALIGN_CENTER, batx, 0);
        lv_obj_set_size(batarc, 60, 60);
        lv_obj_remove_flag(batarc, LV_OBJ_FLAG_EVENT_BUBBLE);

        lv_arc_set_rotation(batarc, 180);

        lv_obj_set_style_arc_width(batarc, 5, LV_PART_MAIN);
        lv_obj_set_style_arc_width(batarc, 5, LV_PART_INDICATOR);

        lv_obj_remove_flag(batarc, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE); /// Flags
        lv_arc_set_bg_angles(batarc, 135, 405);
        lv_arc_set_range(batarc, 0, 100);
        lv_arc_set_value(batarc, 50);

        lv_obj_set_style_bg_opa(batarc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

        baticn = lv_label_create(scr);
        lv_obj_align(baticn, LV_ALIGN_CENTER, batx, -22);
        lv_label_set_text(baticn, LV_SYMBOL_BATTERY_FULL);
    };
    void periodic()
    {
        lv_label_set_text_fmt(minute, "%02i", sysinfo.time.minute);
        lv_label_set_text_fmt(hour, "%02i", sysinfo.time.hour12);
        lv_label_set_text_fmt(batnum, "%i%%", sysinfo.bat.percent);
        lv_arc_set_value(batarc, sysinfo.bat.percent);
    };
    lv_obj_t *scr;
};