#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "notification.hpp"
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
    lv_obj_align(alerttitle, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(alerttitle, &Outfit_32, 0);
    lv_label_set_text(alerttitle, "Alert");
    lv_obj_set_width(alerttitle, 200);

    powermgmRegisterCB(alertperiodic, POWERMGM_LOOP_AWAKE, "alertscreenperiodic");
    return true;
}

bool alertsetup = powermgmRegisterCBPrio(alertcreate, POWERMGM_INIT, "alertscreeninit", CALL_CB_LAST);