#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "notification.hpp"
#include "motor.hpp"
#include "fonts/fonts.hpp"

void nullFunc() {};

lv_obj_t *alertscr;
lv_obj_t *alerttitle;
void (*callback)() = nullFunc;

void showAlert(String text, void (*cb)())
{
    lv_label_set_text(alerttitle, text.c_str());
    callback = cb;
    setScreen(alertscr, LV_SCR_LOAD_ANIM_FADE_IN);
}

bool alertperiodic(EventBits_t event, void *arg)
{
    return true;
}

bool alertcreate(EventBits_t event, void *arg)
{
    alertscr = lv_obj_create(nullptr);
    addGestureCB(alertscr, LV_DIR_RIGHT, [](lv_event_t *e)
                 { setScreen(nullptr, LV_SCR_LOAD_ANIM_FADE_OUT); callback(); });

    alerttitle = lv_label_create(alertscr);
    lv_obj_align(alerttitle, LV_ALIGN_CENTER, 0, -20);
    lv_obj_set_style_text_align(alerttitle, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(alerttitle, &Outfit_32, 0);
    lv_label_set_text(alerttitle, "Alert");
    lv_obj_set_width(alerttitle, 200);

    lv_obj_t *alertclose = lv_button_create(alertscr);
    lv_obj_set_size(alertclose, 100, 50);
    lv_obj_align(alertclose, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_set_style_radius(alertclose, 25, LV_PART_MAIN);

    lv_obj_t *alertlbl = lv_label_create(alertclose);
    lv_obj_align(alertlbl, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(alertlbl, &Outfit_20, 0);
    lv_label_set_text(alertlbl, "Close");

    lv_obj_add_event_cb(alertclose, [](lv_event_t *e)
                        { setScreen(nullptr, LV_SCR_LOAD_ANIM_FADE_OUT); callback(); }, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(alertclose, [](lv_event_t *e)
                        { motorVibrate(HAPTIC_BUTTON); }, LV_EVENT_PRESSED, NULL);

    powermgmRegisterCB(alertperiodic, POWERMGM_LOOP_AWAKE, "alertscreenperiodic");
    return true;
}

bool alertsetup = powermgmRegisterCBPrio(alertcreate, POWERMGM_INIT, "alertscreeninit", CALL_CB_LAST);