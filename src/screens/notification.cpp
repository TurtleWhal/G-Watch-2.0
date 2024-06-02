#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "fonts/fonts.hpp"
#include "notification.hpp"

lv_obj_t *notifscr;

lv_obj_t *icon;
lv_obj_t *title;
lv_obj_t *body;
Notification_t notif;

lv_obj_t *scrollbar;

uint32_t notifmillis = 0;
bool notifshowing = false;

void showNotification(Notification_t *data)
{
    Log.verboseln("showing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", data->title.c_str(), data->body.c_str(), data->sender.c_str(), String(data->tel_number).c_str(), data->time);

    SET_SYMBOL_40(icon, data->icon.c_str());
    lv_label_set_text(title, data->title.c_str());
    lv_label_set_text(body, data->body.c_str());

    setScreen(notifscr);

    lv_obj_scroll_to_x(notifscr, 0, LV_ANIM_ON);
    lv_obj_send_event(notifscr, LV_EVENT_SCROLL, scrollbar);

    notifmillis = millis();
    notifshowing = true;
    notif = *data;
}

bool notifperiodic(EventBits_t event, void *arg)
{
    if (!notifshowing)
        return true;
    // if (ON_CURRENT_SCREEN(notifx, notify))
    // {
    //     Log.verboseln("on Notif screen");
    // }

    if (notifmillis + 30000 < millis())
    {
        Log.verboseln("hiding notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif.title.c_str(), notif.body.c_str(), notif.sender.c_str(), String(notif.tel_number).c_str(), notif.time);

        storeNotification(&notif);
        setScreen(nullptr, LV_SCR_LOAD_ANIM_FADE_OUT);
        notifshowing = false;
    }
    // else
    //     Serial.println(millis() - notifmillis);

    return true;
}

bool notifcreate(EventBits_t event, void *arg)
{
    notifscr = lv_obj_create(nullptr);
    lv_obj_set_scroll_dir(notifscr, LV_DIR_VER);
    lv_obj_set_flex_flow(notifscr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(notifscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);  
    addGestureCB(notifscr, LV_DIR_RIGHT, [](lv_event_t *e)
                 { notifshowing = false; setScreen(nullptr); });

    scrollbar = createCurvedScrollbar(notifscr);

    lv_obj_t *ts = lv_obj_create(notifscr);
    lv_obj_set_size(ts, 0, 10);

    icon = lv_label_create(notifscr);
    SET_SYMBOL_40(icon, FA_BELL)

    title = lv_label_create(notifscr);
    lv_label_set_text(title, "Notification Title");
    lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);
    lv_obj_set_width(title, 140);

    body = lv_label_create(notifscr);
    lv_obj_set_width(body, 160);
    lv_label_set_text(body, "Notification Body");

    lv_obj_t *bs = lv_obj_create(notifscr);
    lv_obj_set_size(bs, 0, 20);

    powermgmRegisterCB(notifperiodic, POWERMGM_LOOP_AWAKE, "notifscreeninit");
    return true;
}

bool notifsetup = powermgmRegisterCBPrio(notifcreate, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);