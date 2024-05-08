#include "lvgl.h"

#define ON_CURRENT_SCREEN(x, y) (lv_obj_get_scroll_x(lv_scr_act()) == x * TFT_WIDTH && lv_obj_get_scroll_y(lv_scr_act()) == y * TFT_HEIGHT)

lv_obj_t *screenCreate(int8_t, int8_t);