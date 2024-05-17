#include "lvgl.h"

#define SCREEN_SCROLL_OFFSET 1
#define ON_CURRENT_SCREEN(x, y) (lv_obj_get_scroll_x(lv_scr_act()) == (x + SCREEN_SCROLL_OFFSET) * TFT_WIDTH && lv_obj_get_scroll_y(lv_scr_act()) == (y + SCREEN_SCROLL_OFFSET) * TFT_HEIGHT)

#define KEYBOARD_CHARS_ALL " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+`-={}|[]\\:;\"'<>,.?/"
#define KEYBOARD_CHARS_NUMBERS "0123456789"
#define KEYBOARD_CHARS_UPPERCASE " ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define KEYBOARD_CHARS_LOWERCASE " abcdefghijklmnopqrstuvwxyz"
#define KEYBOARD_CHARS_ABC " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
#define KEYBOARD_CHARS_SPECIAL " !@#$%^&*()_+`-={}|[]\\:;\"'<>,.?/"

lv_obj_t *screenCreate(int8_t, int8_t);
void setScreen(lv_obj_t *, lv_screen_load_anim_t = LV_SCR_LOAD_ANIM_FADE_ON);
void setScroll(lv_dir_t);
void addGestureCB(lv_obj_t *, lv_dir_t, lv_event_cb_t);
void curvedScrollbarCB(lv_event_t *);
lv_obj_t *createCurvedScrollbar(lv_obj_t *);

// LVGL Object, Accepted Chars
void attachKeyboard(lv_obj_t *, char * = KEYBOARD_CHARS_ALL);