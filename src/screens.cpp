// #include "screens/screens.hpp"
#include "screens/numbers.hpp"
#include "screens/clock.hpp"
// #include "lvgl.h"
// #include "system.hpp"

ClockScreen clockscreen;
NumbersScreen numbersscreen;

lv_obj_t *scr;

void screenInit()
{

    scr = lv_obj_create(nullptr);
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);

    lv_obj_set_scroll_snap_x(scr, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scroll_snap_y(scr, LV_SCROLL_SNAP_CENTER);
    lv_obj_update_snap(scr, LV_ANIM_ON);

    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    numbersscreen.create(scr, 0, 0);
    clockscreen.create(scr, 1, 0);
    lv_screen_load(scr);
}

void screenPeriodic()
{
    numbersscreen.periodic();
    clockscreen.periodic();
}

lv_obj_t *screenCreate(uint8_t x, uint8_t y)
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