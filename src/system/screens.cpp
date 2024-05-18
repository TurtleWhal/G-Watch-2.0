#include "ArduinoLog.h"

#include "powermgm.hpp"
#include "lvgl.h"
#include "screens/screens.hpp"
#include "fonts/fonts.hpp"
#include "symbols.h"

#define SCROLLBAR_WIDTH 30
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

    lv_obj_add_event_cb(scr, [](lv_event_t *e)
                        { powermgmSendEvent(POWERMGM_SCREEN_CHANGE); }, LV_EVENT_SCROLL_END, NULL);

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

void setScroll(lv_dir_t dir)
{
    lv_obj_set_scroll_dir(scr, dir);
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
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *bar = lv_arc_create(scr);
    lv_obj_set_size(bar, 230, 230);
    lv_obj_center(bar);

    lv_obj_add_flag(bar, LV_OBJ_FLAG_FLOATING);

    lv_obj_set_style_arc_color(bar, lv_color_hex(0xbbbbbb), LV_PART_MAIN);
    lv_obj_set_style_arc_opa(bar, LV_OPA_40, LV_PART_MAIN);
    lv_obj_set_style_arc_color(bar, lv_color_hex(0xbbbbbb), LV_PART_INDICATOR);

    lv_obj_set_style_arc_width(bar, 5, LV_PART_MAIN);
    lv_obj_set_style_arc_width(bar, 5, LV_PART_INDICATOR);

    lv_obj_remove_style(bar, NULL, LV_PART_KNOB);
    lv_obj_remove_flag(bar, LV_OBJ_FLAG_CLICKABLE);

    lv_arc_set_bg_angles(bar, SCROLLBAR_START, SCROLLBAR_START + SCROLLBAR_WIDTH);

    lv_obj_scroll_to_x(scr, 0, LV_ANIM_OFF);

    lv_obj_add_event_cb(scr, curvedScrollbarCB, LV_EVENT_SCROLL, bar);
    // lv_obj_send_event(scr, LV_EVENT_SCROLL, bar);

    return bar;
}

void curvedScrollbarCB(lv_event_t *e)
{
    lv_obj_t *scr = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *bar = (lv_obj_t *)lv_event_get_user_data(e);

    int height = lv_obj_get_scroll_top(scr) + lv_obj_get_scroll_bottom(scr) + TFT_HEIGHT;

    Log.verboseln("Scrollbar Height: %i, Scroll Y: %i", height, lv_obj_get_scroll_y(scr));

    if (height <= TFT_HEIGHT)
    {
        lv_obj_add_flag(bar, LV_OBJ_FLAG_HIDDEN);
        return;
    }

    lv_obj_remove_flag(bar, LV_OBJ_FLAG_HIDDEN);

    double mod = (SCROLLBAR_WIDTH + 0.0) / (height + 0.0);

    int start = SCROLLBAR_START + (lv_obj_get_scroll_y(scr) * mod);
    int end = SCROLLBAR_START + ((lv_obj_get_scroll_y(scr) + TFT_HEIGHT) * mod);

    lv_arc_set_range(bar, 0, height);
    lv_arc_set_angles(bar, constrain(start, SCROLLBAR_START, SCROLLBAR_START + SCROLLBAR_WIDTH), constrain(end, SCROLLBAR_START, SCROLLBAR_START + SCROLLBAR_WIDTH));

    // if (lv_obj_is_scrolling(scr))
    // {
    //     lv_obj_set_style_arc_opa(bar, LV_OPA_40, LV_PART_MAIN);
    //     lv_obj_set_style_arc_opa(bar, LV_OPA_100, LV_PART_INDICATOR);
    // } else {
    //     lv_obj_set_style_arc_opa(bar, LV_OPA_40, LV_PART_MAIN);
    //     lv_obj_set_style_arc_opa(bar, LV_OPA_60, LV_PART_INDICATOR);
    // }
}

char *chars = KEYBOARD_CHARS_ALL;
uint8_t charLength = strlen(chars);
uint8_t charIdx = 1;

void createKeyboard(lv_obj_t *scr, lv_obj_t *dest, char *accepted)
{
    chars = accepted;
    charLength = strlen(accepted);
    charIdx = 1;

    lv_obj_t *kb = lv_obj_create(scr);
    lv_obj_t *text;

    lv_obj_set_style_bg_opa(kb, LV_OPA_80, LV_PART_MAIN);
    lv_obj_set_size(kb, 240, 240);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_style_border_opa(kb, LV_OPA_0, LV_PART_MAIN);
    lv_obj_center(kb);
    lv_obj_set_scroll_dir(kb, LV_DIR_NONE);

    lv_obj_t *arc = lv_arc_create(kb);
    lv_arc_set_bg_angles(arc, 20, 160);
    lv_obj_set_size(arc, 220, 220);
    lv_obj_center(arc);
    lv_arc_set_range(arc, 0, charLength);
    lv_arc_set_value(arc, 0);
    lv_arc_set_mode(arc, LV_ARC_MODE_REVERSE);
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_ADV_HITTEST);

    lv_obj_t *nxt = lv_button_create(kb);
    lv_obj_t *prv = lv_button_create(kb);

    lv_obj_set_size(nxt, 50, 50);
    lv_obj_set_size(prv, 50, 50);
    lv_obj_set_style_radius(prv, 25, LV_PART_MAIN);
    lv_obj_set_style_radius(nxt, 25, LV_PART_MAIN);

    lv_obj_align(prv, LV_ALIGN_CENTER, -45, 40);
    lv_obj_align(nxt, LV_ALIGN_CENTER, 45, 40);

    lv_obj_t *nxtlbl = lv_label_create(nxt);
    lv_obj_t *curlbl = lv_label_create(kb);
    lv_obj_t *prvlbl = lv_label_create(prv);

    lv_obj_center(nxtlbl);
    lv_obj_align(curlbl, LV_ALIGN_CENTER, 0, 50);
    lv_obj_center(prvlbl);

    lv_obj_set_style_text_font(nxtlbl, &Outfit_32, 0);
    lv_obj_set_style_text_font(curlbl, &Outfit_32, 0);
    lv_obj_set_style_text_font(prvlbl, &Outfit_32, 0);

    lv_label_set_text_fmt(nxtlbl, "%c", chars[charIdx + 1 % charLength]);
    lv_label_set_text_fmt(curlbl, "%c", chars[charIdx]);
    lv_label_set_text_fmt(prvlbl, "%c", chars[charIdx - 1 % charLength]);

    lv_obj_t *accept = lv_button_create(kb);
    lv_obj_t *apply = lv_button_create(kb);
    lv_obj_t *back = lv_button_create(kb);
    lv_obj_t *del = lv_button_create(kb);

    lv_obj_set_size(accept, 50, 50);
    lv_obj_set_size(apply, 50, 50);
    lv_obj_set_size(back, 40, 40);
    lv_obj_set_size(del, 40, 40);

    lv_obj_set_style_radius(accept, 25, LV_PART_MAIN);
    lv_obj_set_style_radius(apply, 25, LV_PART_MAIN);
    lv_obj_set_style_radius(back, 20, LV_PART_MAIN);
    lv_obj_set_style_radius(del, 20, LV_PART_MAIN);

    lv_obj_align(accept, LV_ALIGN_CENTER, 80, -10);
    lv_obj_align(apply, LV_ALIGN_CENTER, -80, -10);
    lv_obj_align(back, LV_ALIGN_CENTER, -65, -65);
    lv_obj_align(del, LV_ALIGN_CENTER, 65, -65);

    lv_obj_t *acceptlbl = lv_label_create(accept);
    lv_obj_t *applylbl = lv_label_create(apply);
    lv_obj_t *backlbl = lv_label_create(back);
    lv_obj_t *dellbl = lv_label_create(del);

    lv_obj_center(acceptlbl);
    lv_obj_center(applylbl);
    lv_obj_center(backlbl);
    lv_obj_center(dellbl);

    SET_SYMBOL_20(acceptlbl, LV_SYMBOL_UP);
    SET_SYMBOL_20(applylbl, LV_SYMBOL_OK);
    SET_SYMBOL_20(backlbl, LV_SYMBOL_CLOSE);
    SET_SYMBOL_20(dellbl, LV_SYMBOL_BACKSPACE);

    text = lv_textarea_create(kb);
    lv_obj_align(text, LV_ALIGN_CENTER, 0, -15);
    lv_obj_set_style_radius(text, 15, LV_PART_MAIN);
    lv_obj_set_style_bg_color(text, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_size(text, 100, 30);
    lv_obj_set_style_text_font(text, &Outfit_20, 0);
    lv_textarea_set_one_line(text, true);
    lv_textarea_set_text(text, lv_textarea_get_text(dest));

    // attach callbacks

    text->user_data = curlbl;
    dest->user_data = text;

    lv_obj_add_event_cb(apply, [](lv_event_t *e)
                        {
                            lv_textarea_set_text((lv_obj_t *)e->user_data, lv_textarea_get_text((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data));
                            lv_obj_delete(lv_obj_get_parent((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)); }, LV_EVENT_CLICKED, dest);

    lv_obj_add_event_cb(accept, [](lv_event_t *e)
                        { lv_textarea_add_char((lv_obj_t *)e->user_data, lv_label_get_text((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)[0]); }, LV_EVENT_CLICKED, text);

    lv_obj_add_event_cb(back, [](lv_event_t *e)
                        { lv_obj_delete((lv_obj_t *)e->user_data); }, LV_EVENT_CLICKED, kb);

    lv_obj_add_event_cb(del, [](lv_event_t *e)
                        { lv_textarea_delete_char((lv_obj_t *)e->user_data); }, LV_EVENT_CLICKED, text);

    nxtlbl->user_data = prvlbl;
    prvlbl->user_data = curlbl;
    curlbl->user_data = arc;

    lv_obj_add_event_cb(nxt, [](lv_event_t *e)
                        {
                            charIdx++;
                            charIdx %= charLength;

                            lv_label_set_text_fmt((lv_obj_t *)e->user_data, "%c", chars[(charIdx + 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data, "%c", chars[(charIdx - 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)->user_data, "%c", chars[charIdx]);

                            lv_arc_set_value((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)->user_data)->user_data, charIdx); }, LV_EVENT_CLICKED, nxtlbl);

    lv_obj_add_event_cb(prv, [](lv_event_t *e)
                        {
                            charIdx--;
                            charIdx %= charLength;

                            lv_label_set_text_fmt((lv_obj_t *)e->user_data, "%c", chars[(charIdx + 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data, "%c", chars[(charIdx - 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)->user_data, "%c", chars[charIdx]);

                            lv_arc_set_value((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)->user_data)->user_data, charIdx); }, LV_EVENT_CLICKED, nxtlbl);

    lv_obj_add_event_cb(arc, [](lv_event_t *e)
                        {
                            charIdx = lv_arc_get_value((lv_obj_t *)e->original_target) % charLength;

                            lv_label_set_text_fmt((lv_obj_t *)e->user_data, "%c", chars[(charIdx + 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data, "%c", chars[(charIdx - 1) % charLength]);
                            lv_label_set_text_fmt((lv_obj_t *)((lv_obj_t *)((lv_obj_t *)e->user_data)->user_data)->user_data, "%c", chars[charIdx]); }, LV_EVENT_VALUE_CHANGED, nxtlbl);
}

void attachKeyboard(lv_obj_t *textarea, char *accepted)
{
    lv_obj_add_event_cb(textarea, [](lv_event_t *e)
                        { createKeyboard(lv_obj_get_screen(lv_event_get_target_obj(e)), lv_event_get_target_obj(e), (char *)e->user_data); }, LV_EVENT_CLICKED, accepted);
}

bool screensetup = powermgmRegisterCBPrio(screenInit, POWERMGM_INIT, "ScreenInit", CALL_CB_MIDDLE);