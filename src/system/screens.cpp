#include "ArduinoLog.h"

#include "powermgm.hpp"
#include "lvgl.h"
#include "screens/screens.hpp"

#define SCROLLBAR_WIDTH 90
#define SCROLLBAR_START (360 - SCROLLBAR_WIDTH / 2)

lv_obj_t *scr;

bool screenPeriodic(EventBits_t event, void *arg)
{
    // Log.verboseln("Scroll x: %i, y: %i", lv_obj_get_scroll_x(scr), lv_obj_get_scroll_y(scr));

    return true;
}

bool screenInit(EventBits_t event, void *arg)
{
    scr = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);

    lv_obj_set_scroll_snap_x(scr, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scroll_snap_y(scr, LV_SCROLL_SNAP_CENTER);
    lv_obj_update_snap(scr, LV_ANIM_ON);

    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_screen_load(scr);

    powermgmRegisterCB(screenPeriodic, POWERMGM_LOOP_AWAKE, "ScreenPeriodic");
    return true;
}

lv_obj_t *screenCreate(int8_t x, int8_t y)
{

    x += SCREEN_SCROLL_OFFSET;
    y += SCREEN_SCROLL_OFFSET;

    lv_obj_t *screen = lv_obj_create(scr);

    lv_obj_set_size(screen, TFT_WIDTH, TFT_HEIGHT);
    lv_obj_align(screen, LV_ALIGN_CENTER, TFT_WIDTH * x, TFT_HEIGHT * y);
    lv_obj_set_style_bg_opa(screen, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_radius(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_opa(screen, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_scroll_dir(screen, LV_DIR_NONE);

    lv_obj_scroll_to_x(scr, TFT_WIDTH * SCREEN_SCROLL_OFFSET, LV_ANIM_OFF);
    lv_obj_scroll_to_y(scr, TFT_HEIGHT * SCREEN_SCROLL_OFFSET, LV_ANIM_OFF);

    return screen;
}

void setScreen(lv_obj_t *screen, lv_screen_load_anim_t anim)
{
    if (screen == nullptr)
        screen = scr;

    lv_screen_load_anim(screen, anim, 100, 0, 0);
}

lv_dir_t gestdir;
lv_event_cb_t gestcb;

void addGestureCB(lv_obj_t *obj, lv_dir_t dir, lv_event_cb_t cb)
{
    gestdir = dir;
    gestcb = cb;

    lv_obj_add_event_cb(obj, [](lv_event_t *e)
                        {lv_dir_t direction = lv_indev_get_gesture_dir(lv_indev_get_act()); if (direction == gestdir) gestcb(e); }, LV_EVENT_GESTURE, nullptr);
}

lv_obj_t *createCurvedScrollbar(lv_obj_t *scr)
{
    lv_obj_t *bar = lv_arc_create(scr);
    lv_obj_set_size(bar, 230, 230);
    lv_obj_center(bar);

    lv_obj_set_style_arc_color(bar, lv_color_hex(0x888888), LV_PART_MAIN);
    lv_obj_set_style_arc_color(bar, lv_color_hex(0xeeeeee), LV_PART_INDICATOR);

    lv_obj_set_style_arc_width(bar, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width(bar, 5, LV_PART_INDICATOR);

    lv_obj_remove_style(bar, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(bar, LV_OBJ_FLAG_CLICKABLE);

    lv_arc_set_bg_angles(bar, SCROLLBAR_START, SCROLLBAR_START + SCROLLBAR_WIDTH);

    lv_arc_set_range(bar, 0, lv_obj_get_self_height(scr) + TFT_HEIGHT);

    // lv_arc_set_rotation(bar, 270);

    // lv_arc_set_angles(bar, SCROLLBAR_START + lv_obj_get_scroll_y(bar), SCROLLBAR_START + lv_obj_get_scroll_y(bar) + TFT_HEIGHT);
    // lv_arc_set_angles(bar, lv_obj_get_scroll_y(bar), lv_obj_get_scroll_y(bar) + TFT_HEIGHT);
    // lv_arc_set_angles(bar, -20, 20);

    return bar;
}

void curvedScrollbarCB(lv_event_t *e)
{
    lv_obj_t *scr = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *bar = (lv_obj_t *)lv_event_get_user_data(e);

    double mod = 90 / (lv_obj_get_self_height(scr) + TFT_HEIGHT);
    // double mod = 0.1875;

    Log.verbose("Scroll: %i, Height: %i, Mod: ", lv_obj_get_scroll_y(scr), lv_obj_get_self_height(scr) + TFT_HEIGHT);
    Serial.println(90 / (lv_obj_get_height(scr) + TFT_HEIGHT));

    lv_arc_set_range(bar, 0, lv_obj_get_self_height(scr) + TFT_HEIGHT);
    lv_arc_set_angles(bar, SCROLLBAR_START + (lv_obj_get_scroll_y(scr) * mod), SCROLLBAR_START + (lv_obj_get_scroll_y(scr) + TFT_HEIGHT) * mod);
    // lv_arc_set_angles(bar, SCROLLBAR_START + 10, SCROLLBAR_START + 20);
    // lv_arc_set_value(bar, lv_obj_get_scroll_y(scr));

    lv_obj_set_y(bar, lv_obj_get_scroll_y(scr));
}

bool screensetup = powermgmRegisterCBPrio(screenInit, POWERMGM_INIT, "ScreenInit", CALL_CB_MIDDLE);