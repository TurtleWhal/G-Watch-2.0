#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "notification.hpp"

lv_obj_t *notifscr;

lv_obj_t *title;
lv_obj_t *body;
Notification_t notif;

lv_obj_t *scrollbar;

uint32_t notifmillis = 0;
bool notifshowing = false;

void showNotification(Notification_t *data)
{
    Log.verboseln("showing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", data->title.c_str(), data->body.c_str(), data->sender.c_str(), String(data->tel_number).c_str(), data->time);

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

    if (notifmillis + 5000 < millis())
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
    addGestureCB(notifscr, LV_DIR_RIGHT, [](lv_event_t *e)
                 { notifshowing = false; setScreen(nullptr); });

    scrollbar = createCurvedScrollbar(notifscr);

    title = lv_label_create(notifscr);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -80);
    lv_label_set_text(title, "Notification Title");

    body = lv_label_create(notifscr);
    lv_obj_align(body, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_width(body, 160);
    lv_label_set_text(body, "Notification Body");

    powermgmRegisterCB(notifperiodic, POWERMGM_LOOP_AWAKE, "notifscreeninit");
    return true;
}

bool notifsetup = powermgmRegisterCBPrio(notifcreate, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);