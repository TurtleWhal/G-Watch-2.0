#include "lvgl.h"

#define SCREEN_SCROLL_OFFSET 1
#define ON_CURRENT_SCREEN(x, y) (lv_obj_get_scroll_x(lv_scr_act()) == (x + SCREEN_SCROLL_OFFSET) * TFT_WIDTH && lv_obj_get_scroll_y(lv_scr_act()) == (y + SCREEN_SCROLL_OFFSET) * TFT_HEIGHT)

lv_obj_t *screenCreate(int8_t, int8_t);
void setScreen(lv_obj_t *, lv_screen_load_anim_t = LV_SCR_LOAD_ANIM_FADE_ON);
void addGestureCB(lv_obj_t *, lv_dir_t, lv_event_cb_t);
void curvedScrollbarCB(lv_event_t *);
lv_obj_t *createCurvedScrollbar(lv_obj_t *);