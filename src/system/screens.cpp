#include "ArduinoLog.h"
#include "screens/numbers.hpp"
#include "screens/clock.hpp"
#include "screens/music.hpp"

#include "powermgm.hpp"
// #include "lvgl.h"
// #include "system.hpp"

ClockScreen clockscreen;
NumbersScreen numbersscreen;
MusicScreen musicscreen;

lv_obj_t *scr;

bool screenPeriodic(EventBits_t event, void *arg)
{
    numbersscreen.periodic();
    clockscreen.periodic();
    musicscreen.periodic();

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

    numbersscreen.create(scr, 0, 0);
    clockscreen.create(scr, 1, 0);
    musicscreen.create(scr, 0, 1);
    lv_screen_load(scr);

    powermgmRegisterCB(screenPeriodic, POWERMGM_LOOP, "ExampleFunc");
    return true;
}

lv_obj_t *screenCreate(int8_t x, int8_t y)
{

    lv_obj_t *screen = lv_obj_create(scr);

    lv_obj_set_size(screen, TFT_WIDTH, TFT_HEIGHT);
    lv_obj_align(screen, LV_ALIGN_CENTER, TFT_WIDTH * x, TFT_HEIGHT * y);
    lv_obj_set_style_bg_opa(screen, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_radius(screen, 0, LV_PART_MAIN);
    lv_obj_set_style_border_opa(screen, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_scroll_dir(screen, LV_DIR_NONE);

    return screen;
}

bool screensetup = powermgmRegisterCBPrio(screenInit, POWERMGM_INIT, "ExampleFunc", CALL_CB_MIDDLE);