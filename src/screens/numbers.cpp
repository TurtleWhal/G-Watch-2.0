#include "Arduino.h"
#include "lvgl.h"
#include "powermgm.hpp"
#include "system.hpp"
#include "screens.hpp"
#include "fonts/fonts.hpp"

void nullCallback(uint8_t arcid) {}

typedef struct
{
    lv_obj_t *lbl;
    lv_obj_t *icon;
    lv_obj_t *arc;
    // lv_img_dsc_t img;
    String symbol = LV_SYMBOL_DUMMY;

    void (*updateCB)(uint8_t) = nullCallback;
} InfoArc_t;

lv_obj_t *numberscr;

lv_obj_t *hour;
lv_obj_t *minute;
lv_obj_t *date;

InfoArc_t arcs[3];
uint8_t arcsize = 55;

int8_t numberx = 0, numbery = 0;

void createArc(uint8_t arcid, int32_t x, int32_t y, char *symbol, void (*updateCB)(uint8_t))
{
    InfoArc_t *arc = &arcs[arcid];

    arc->arc = lv_arc_create(numberscr);
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

    arc->lbl = lv_label_create(numberscr);
    lv_obj_align(arc->lbl, LV_ALIGN_CENTER, x, y);

    // arc->icon = lv_img_create(numberscr);
    // lv_obj_align(arc->icon, LV_ALIGN_CENTER, x, y - 22);
    // lv_img_set_src(arc->icon, &arc->img);
    arc->icon = lv_label_create(numberscr);
    lv_obj_align(arc->icon, LV_ALIGN_CENTER, x, y - (arcsize / 3));
    // lv_label_set_text(arc->icon, arc->symbol.c_str());
    arc->symbol = symbol;
    SET_SYMBOL_14(arc->icon, arc->symbol.c_str());

    // updateCBs[arcid] = updateCB;
    arc->updateCB = updateCB;
}

void scheduleCallback(uint8_t arcid)
{
    InfoArc_t *arc = &arcs[arcid];
    // lv_arc_set_value(arc->arc, sysinfo.schedule.percent);
    // lv_label_set_text_fmt(arc->arc, "%s", sysinfo.schedule.next.c_str());
    lv_arc_set_value(arc->arc, 0);
    lv_label_set_text_fmt(arc->lbl, "%i", 0);
}

void stepsCallback(uint8_t arcid)
{
    InfoArc_t *arc = &arcs[arcid];
    static uint32_t laststeps = 0;
    if (laststeps != sysinfo.health.steps)
    {
        lv_arc_set_value(arc->arc, sysinfo.health.steps);
        lv_label_set_text_fmt(arc->lbl, "%i", sysinfo.health.steps);
        laststeps = sysinfo.health.steps;
    }
}

void batteryCallback(uint8_t arcid)
{
    InfoArc_t *arc = &arcs[arcid];
    static uint32_t lastbat = 0;
    if (lastbat != sysinfo.bat.percent)
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
                SET_SYMBOL_14(arc->icon, LV_SYMBOL_BATTERY_FULL);
                break;
            case 4:
                SET_SYMBOL_14(arc->icon, LV_SYMBOL_BATTERY_3);
                break;
            case 3:
                SET_SYMBOL_14(arc->icon, LV_SYMBOL_BATTERY_2);
                break;
            case 2:
                SET_SYMBOL_14(arc->icon, LV_SYMBOL_BATTERY_1);
                break;
            case 1:
                SET_SYMBOL_14(arc->icon, LV_SYMBOL_BATTERY_EMPTY);
                break;
            }
        }
    }
}

bool numbersperiodic(EventBits_t event, void *arg)
{
    if (ON_CURRENT_SCREEN(numberx, numbery))
    {
        static uint8_t lastmin;
        if (lastmin != sysinfo.time.minute)
        {
            lv_label_set_text_fmt(minute, "%02i", sysinfo.time.minute);
            lv_label_set_text_fmt(hour, "%02i", sysinfo.time.hour12);

            String colored = sysinfo.date.strdate;

            // ***** colored text not yet supported in lvgl 9.1 *****
            // colored = "#0000ff" + colored.substring(0, 3) + "#" + colored.substring(3);
            // lv_color_t c = lv_theme_get_color_primary(numberscr);
            // char hex[8] = "#0000ff";
            // sprintf(hex, "#%02X%02X%02X", c.red, c.green, c.blue);
            // colored.replace("#0000ff", hex);

            lv_label_set_text(date, colored.c_str());
            lastmin = sysinfo.time.minute;
        }

        // scheduleCallback(&arcs[0]);
        // stepsCallback(&arcs[1]);
        // batteryCallback(&arcs[2]);

        // updateCBs[0](&arcs[0]);
        // updateCBs[1](&arcs[1]);
        // updateCBs[2](&arcs[2]);

        arcs[0].updateCB(0);
        arcs[1].updateCB(1);
        arcs[2].updateCB(2);
    }

    return true;
}

bool numberscreate(EventBits_t event, void *arg)
{
    numberscr = screenCreate(numberx, numbery);

    hour = lv_label_create(numberscr);
    minute = lv_label_create(numberscr);

    lv_label_set_text(hour, "00");
    lv_label_set_text(minute, "00");

    lv_obj_align(hour, LV_ALIGN_CENTER, 30, -65);
    lv_obj_align(minute, LV_ALIGN_CENTER, 30, 5);

    lv_obj_set_style_text_font(hour, &Outfit_80_Light, 0);
    lv_obj_set_style_text_font(minute, &Outfit_80_Light, 0);

    lv_obj_set_style_text_color(hour, lv_color_lighten(lv_theme_get_color_primary(numberscr), 100), 0);
    lv_obj_set_style_text_color(minute, lv_color_lighten(lv_theme_get_color_primary(numberscr), 200), 0);

    date = lv_label_create(numberscr);
    lv_obj_align(date, LV_ALIGN_CENTER, 30, 55);
    lv_label_set_text(date, "Mon Jan 1");
    lv_obj_set_style_text_font(date, &Outfit_20, 0);

    // arcs[0].symbol = FA_CALENDAR;
    // arcs[1].symbol = FA_STEPS;
    // arcs[2].symbol = LV_SYMBOL_BATTERY_FULL;

    // createArc(&arcs[0], -58, -64);
    // createArc(&arcs[1], -85, 0);
    // createArc(&arcs[2], -58, 64);

    createArc(0, -85, 00, FA_STEPS, stepsCallback);
    createArc(1, -58, 64, LV_SYMBOL_BATTERY_FULL, batteryCallback);

    powermgmRegisterCB(numbersperiodic, POWERMGM_LOOP_AWAKE, "numbersscreenperiodic");

    return true;
}

bool numberssetup = powermgmRegisterCBPrio(numberscreate, POWERMGM_INIT, "numbersscreencreate", CALL_CB_LAST);