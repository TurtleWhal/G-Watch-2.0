#include "Arduino.h"
#include "lvgl.h"
#include "system.hpp"

LV_FONT_DECLARE(RedditSans_80_Light);
LV_FONT_DECLARE(RedditSans_20_Light);

typedef struct
{
    lv_obj_t *lbl;
    lv_obj_t *icon;
    lv_obj_t *arc;
    // lv_img_dsc_t img;
    String symbol = LV_SYMBOL_DUMMY;
} InfoArc_t;

class NumbersScreen
{
private:
    lv_obj_t *hour;
    lv_obj_t *minute;
    lv_obj_t *date;

    InfoArc_t arcs[3];
    uint8_t arcsize = 55;

    void createArc(InfoArc_t *arc, int32_t x, int32_t y)
    {
        arc->arc = lv_arc_create(scr);
        lv_obj_align(arc->arc, LV_ALIGN_CENTER, x, y);
        lv_obj_set_size(arc->arc, arcsize, arcsize);
        lv_obj_remove_flag(arc->arc, LV_OBJ_FLAG_EVENT_BUBBLE);

        lv_arc_set_rotation(arc->arc, 180);

        lv_obj_set_style_arc_width(arc->arc, 4, LV_PART_MAIN);
        lv_obj_set_style_arc_width(arc->arc, 4, LV_PART_INDICATOR);

        lv_obj_remove_flag(arc->arc, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_CLICK_FOCUSABLE); /// Flags
        lv_arc_set_bg_angles(arc->arc, 135, 405);
        lv_arc_set_range(arc->arc, 0, 100);
        lv_arc_set_value(arc->arc, 50);

        lv_obj_set_style_bg_opa(arc->arc, 0, LV_PART_KNOB | LV_STATE_DEFAULT);

        arc->lbl = lv_label_create(scr);
        lv_obj_align(arc->lbl, LV_ALIGN_CENTER, x, y);

        // arc->icon = lv_img_create(scr);
        // lv_obj_align(arc->icon, LV_ALIGN_CENTER, x, y - 22);
        // lv_img_set_src(arc->icon, &arc->img);
        arc->icon = lv_label_create(scr);
        lv_obj_align(arc->icon, LV_ALIGN_CENTER, x, y - (arcsize / 3));
        lv_label_set_text(arc->icon, arc->symbol.c_str());
    };

    void scheduleCallback(InfoArc_t *arc)
    {
        // lv_arc_set_value(arc->arc, sysinfo.schedule.percent);
        // lv_label_set_text_fmt(arc->arc, "%s", sysinfo.schedule.next.c_str());
        lv_arc_set_value(arc->arc, 0);
        lv_label_set_text_fmt(arc->lbl, "%i", 0);
    }

    void stepsCallback(InfoArc_t *arc)
    {
        // lv_arc_set_value(arc->arc, sysinfo.health.steps);
        // lv_label_set_text_fmt(arc->arc, "%i", sysinfo.health.steps);
        lv_arc_set_value(arc->arc, 0);
        lv_label_set_text_fmt(arc->lbl, "%i", 0);
    }

    void batteryCallback(InfoArc_t *arc)
    {
        lv_arc_set_value(arc->arc, (int32_t)sysinfo.bat.percent);
        lv_label_set_text_fmt(arc->lbl, "%i%%", sysinfo.bat.percent);

        if (sysinfo.bat.charging)
        {
            lv_label_set_text(arc->icon, LV_SYMBOL_CHARGE);
        }
        else
        {
            switch ((int)(sysinfo.bat.percent / 20 + 1))
            {
            default:
                lv_label_set_text(arc->icon, LV_SYMBOL_BATTERY_FULL);
                break;
            case 4:
                lv_label_set_text(arc->icon, LV_SYMBOL_BATTERY_3);
                break;
            case 3:
                lv_label_set_text(arc->icon, LV_SYMBOL_BATTERY_2);
                break;
            case 2:
                lv_label_set_text(arc->icon, LV_SYMBOL_BATTERY_1);
                break;
            case 1:
                lv_label_set_text(arc->icon, LV_SYMBOL_BATTERY_EMPTY);
                break;
            }
        }
    }

public:
    void create(lv_obj_t *screen, uint8_t x, uint8_t y)
    {
        scr = lv_obj_create(screen);

        lv_obj_set_size(scr, TFT_WIDTH, TFT_HEIGHT);
        lv_obj_align(scr, LV_ALIGN_CENTER, TFT_WIDTH * x, TFT_HEIGHT * y);

        hour = lv_label_create(scr);
        minute = lv_label_create(scr);

        lv_label_set_text(hour, "00");
        lv_label_set_text(minute, "00");

        lv_obj_align(hour, LV_ALIGN_CENTER, 30, -65);
        lv_obj_align(minute, LV_ALIGN_CENTER, 30, 5);

        lv_obj_set_style_text_font(hour, &RedditSans_80_Light, 0);
        lv_obj_set_style_text_font(minute, &RedditSans_80_Light, 0);

        lv_obj_set_style_text_color(hour, lv_color_lighten(lv_theme_get_color_primary(scr), 100), 0);
        lv_obj_set_style_text_color(minute, lv_color_lighten(lv_theme_get_color_primary(scr), 200), 0);

        date = lv_label_create(scr);
        lv_obj_align(date, LV_ALIGN_CENTER, 30, 55);
        lv_label_set_text(date, "Mon Jan 1");
        lv_obj_set_style_text_font(date, &RedditSans_20_Light, 0);

        arcs[0].symbol = LV_SYMBOL_LIST;
        arcs[1].symbol = LV_SYMBOL_IMAGE;
        arcs[2].symbol = LV_SYMBOL_BATTERY_FULL;

        createArc(&arcs[0], -58, -64);
        createArc(&arcs[1], -85, 0);
        createArc(&arcs[2], -58, 64);
    };

    void periodic()
    {
        lv_label_set_text_fmt(minute, "%02i", sysinfo.time.minute);
        lv_label_set_text_fmt(hour, "%02i", sysinfo.time.hour12);

        String colored = sysinfo.date.strdate;

        // ***** colored text not yet supported in lvgl 9.1 *****
        // colored = "#0000ff" + colored.substring(0, 3) + "#" + colored.substring(3);
        // lv_color_t c = lv_theme_get_color_primary(scr);
        // char hex[8] = "#0000ff";
        // sprintf(hex, "#%02X%02X%02X", c.red, c.green, c.blue);
        // colored.replace("#0000ff", hex);

        lv_label_set_text(date, colored.c_str());

        scheduleCallback(&arcs[0]);
        stepsCallback(&arcs[1]);
        batteryCallback(&arcs[2]);
    };

    lv_obj_t *scr;
};