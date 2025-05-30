#pragma GCC diagnostic ignored "-Wwrite-strings"

#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "powermgm.hpp"
#include "system.hpp"
#include "screens.hpp"
#include "fonts/fonts.hpp"
#include "notification.hpp"

#define RADIAL_COORDS(angle, radius) (cos((DEG_TO_RAD) * (angle)) * (radius)), (sin((DEG_TO_RAD) * (angle)) * (radius))
#define ICON_SPACING 10
#define ICON_COUNT 6

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
lv_obj_t *info;
lv_obj_t *infoicon;

InfoArc_t arcs[3];
uint8_t arcsize = 55;

// lv_obj_t *infoicons[ICON_COUNT * 2] = {nullptr};
lv_obj_t *iconobjs[ICON_COUNT] = {nullptr};
// char *icons[ICON_COUNT];
char *icons[ICON_COUNT] = {FA_GADGETBRIDGE, FA_GADGETBRIDGE, FA_GADGETBRIDGE, FA_GADGETBRIDGE, FA_GADGETBRIDGE, FA_GADGETBRIDGE};
bool iconschanged = false;

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

    lv_obj_remove_flag(arc->arc, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(arc->arc, LV_OBJ_FLAG_CLICK_FOCUSABLE);
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
    lv_obj_align(arc->icon, LV_ALIGN_CENTER, x, y - (arcsize / 2.5));

    arc->symbol = symbol;
    SET_SYMBOL_14(arc->icon, arc->symbol.c_str());

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
    static uint32_t laststeps = -1;
    if (laststeps != sysinfo.health.steps)
    {
        lv_arc_set_range(arc->arc, 0, sysinfo.health.goal);
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
        // lv_arc_set_value(arc->arc, (int32_t)sysinfo.bat.percent);
        // lv_label_set_text_fmt(arc->lbl, "%i%%", sysinfo.bat.percent);
        lv_arc_set_value(arc->arc, (int32_t)sysinfo.bat.voltage / 45);
        lv_label_set_text_fmt(arc->lbl, "%i", (int)sysinfo.bat.voltage);

        if (sysinfo.bat.charging)
        {
            SET_SYMBOL_14(arc->icon, FA_CHARGE);
        }
        else
        {
            switch ((int)(sysinfo.bat.percent / 20 + 1))
            {
            default:
                SET_SYMBOL_14(arc->icon, FA_BATTERY_FULL);
                break;
            case 4:
                SET_SYMBOL_14(arc->icon, FA_BATTERY_3);
                break;
            case 3:
                SET_SYMBOL_14(arc->icon, FA_BATTERY_2);
                break;
            case 2:
                SET_SYMBOL_14(arc->icon, FA_BATTERY_1);
                break;
            case 1:
                SET_SYMBOL_14(arc->icon, FA_BATTERY_EMPTY);
                break;
            }
        }
    }
}

bool numbersLoad(EventBits_t event, void *arg)
{

    if (ON_CURRENT_SCREEN(numberx, numbery))
    {
        setScroll(LV_DIR_ALL);
    }

    return true;
}

int notificonidx;

bool numbersperiodic(EventBits_t event, void *arg)
{
    if (ON_CURRENT_SCREEN(numberx, numbery))
    {
        static uint8_t lastmin = -1;
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

        static uint8_t lastsec = -1;
        if (lastsec != sysinfo.time.second)
        {
            for (uint8_t i = 0; i < ICON_COUNT; i++)
                icons[i] = nullptr;

            notificonidx = 0;

            if (sysinfo.ble.connected)
                icons[notificonidx++] = FA_BLUETOOTH;

            if (sysinfo.wifi.connected)
                icons[notificonidx++] = FA_WIFI;

            if (sysinfo.donotdisturb)
                icons[notificonidx++] = FA_DO_NOT_DISTURB;

            forEachNotification([](Notification_t *notif)
                                {
                                    if (notificonidx >= ICON_COUNT)
                                    {
                                        icons[ICON_COUNT - 1] = FA_POINT;
                                        return;
                                    }

                                    icons[notificonidx] = (char *)notif->icon.c_str();
                                    notificonidx++; },
                                true);

            iconschanged = true;

            if (lastsec % 20 == 0)
            {
                uint8_t i = 0;
                do
                {
                    sysinfo.glance.shown = (sysinfo.glance.shown + 1) % 10;

                    if (i++ > 10)
                        break;
                } while (!strcmp(sysinfo.glance.strings[sysinfo.glance.shown].c_str(), ""));
            }

            String t = sysinfo.glance.strings[sysinfo.glance.shown];
            if (strcmp(lv_label_get_text(info), t.c_str()))
            {
                lv_label_set_text(info, t.c_str());
                SET_SYMBOL_16(infoicon, t == "" ? t.c_str() : sysinfo.glance.icons[sysinfo.glance.shown].c_str());
            }

            lastsec = sysinfo.time.second;
        }

        arcs[0].updateCB(0);
        arcs[1].updateCB(1);
        arcs[2].updateCB(2);

        if (iconschanged)
        {
            uint8_t infoiconcount = 0;

            for (uint8_t i = 0; i < ICON_COUNT; i++)
                if (icons[i] != nullptr)
                    infoiconcount++;

            uint8_t idx = 0;

            for (uint8_t i = 0; i < ICON_COUNT; i++)
            {
                if (icons[i] != nullptr)
                {
                    lv_obj_align(iconobjs[idx], LV_ALIGN_CENTER,
                                 RADIAL_COORDS(((90 + ((double)((infoiconcount - 1) * ICON_SPACING) / 2)) - (idx * ICON_SPACING)),
                                               (110)));
                    idx++;
                }

                SET_SYMBOL_14(iconobjs[i], icons[i] == nullptr ? "" : icons[i]);
            }

            iconschanged = false;
        }
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

    // lv_obj_set_style_text_font(hour, &OpenSans_80, 0);
    // lv_obj_set_style_text_font(minute, &OpenSans_80, 0);
    lv_obj_set_style_text_font(hour, &OutfitLight_80, 0);
    lv_obj_set_style_text_font(minute, &OutfitLight_80, 0);

    lv_obj_set_style_text_color(hour, lv_color_lighten(lv_theme_get_color_primary(numberscr), 100), 0);
    lv_obj_set_style_text_color(minute, lv_color_lighten(lv_theme_get_color_primary(numberscr), 200), 0);

    date = lv_label_create(numberscr);
    lv_obj_align(date, LV_ALIGN_CENTER, 30, 55);
    lv_label_set_text(date, "Mon Jan 1");
    lv_obj_set_style_text_font(date, &Outfit_20, 0);

    createArc(0, RADIAL_COORDS(210, 80), FA_STEPS, stepsCallback);
    createArc(1, RADIAL_COORDS(150, 80), FA_BATTERY_FULL, batteryCallback);

    for (uint8_t i = 0; i < ICON_COUNT; i++)
    {
        iconobjs[i] = lv_label_create(numberscr);
        SET_SYMBOL_14(iconobjs[i], "");
    }

    lv_obj_t *infobox = lv_obj_create(numberscr);
    lv_obj_set_style_border_opa(infobox, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(infobox, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_align(infobox, LV_ALIGN_CENTER);
    lv_obj_set_size(infobox, 140, 18);
    lv_obj_set_y(infobox, 80);
    lv_obj_set_style_pad_all(infobox, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(infobox, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(infobox, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scroll_dir(infobox, LV_DIR_NONE);

    infoicon = lv_label_create(infobox);
    lv_obj_set_style_text_color(infoicon, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
    lv_obj_set_style_text_align(infoicon, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);

    info = lv_label_create(infobox);
    lv_obj_set_style_text_color(info, lv_color_hex(0xaaaaaa), LV_PART_MAIN);
    lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN);
    lv_label_set_long_mode(info, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_style_text_font(info, &Outfit_16, LV_PART_MAIN);
    lv_obj_set_style_max_width(info, 120, LV_PART_MAIN);
    // lv_obj_set_height(info, 16);
    // lv_obj_set_style_radius(info, 20, LV_PART_MAIN);
    // lv_obj_set_style_pad_all(info, 0, LV_PART_MAIN);
    // lv_obj_set_style_pad_top(info, -3, LV_PART_MAIN);
    // lv_obj_set_style_pad_bottom(info, 0, LV_PART_MAIN);
    // lv_obj_set_style_border_color(info, lv_color_hex(0x222222), LV_PART_MAIN);
    // lv_obj_set_style_border_opa(info, LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_obj_set_style_border_width(info, 2, LV_PART_MAIN);

    // sysinfo.glance.icons[sysinfo.glance.NOW_PLAYING] = FA_MUSIC_NOTE;
    // sysinfo.glance.strings[sysinfo.glance.NOW_PLAYING] = "Long Song Name • Long Artist Name";
    // sysinfo.glance.strings[sysinfo.glance.NOW_PLAYING] = "1234";

    powermgmRegisterCB(numbersperiodic, POWERMGM_LOOP_AWAKE, "numbersscreenperiodic");
    powermgmRegisterCB(numbersLoad, POWERMGM_SCREEN_CHANGE, "numbersscreenload");

    return true;
}

bool numberssetup = powermgmRegisterCBPrio(numberscreate, POWERMGM_INIT, "numbersscreencreate", CALL_CB_LAST);