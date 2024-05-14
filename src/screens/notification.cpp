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

void showNotification(Notification_t *data)
{
    lv_label_set_text(title, data->title.c_str());

    // data->body += data->body;
    // data->body += data->body;
    // data->body += data->body;
    // data->body += data->body;
    // data->body += data->body;
    // data->body += data->body;

    lv_label_set_text(body, data->body.c_str());

    setScreen(notifscr);
}

bool notifperiodic(EventBits_t event, void *arg)
{
    // if (ON_CURRENT_SCREEN(notifx, notify))
    // {
    //     Log.verboseln("on Notif screen");
    // }

    return true;
}

bool notifcreate(EventBits_t event, void *arg)
{
    notifscr = lv_obj_create(nullptr);
    lv_obj_set_scroll_dir(notifscr, LV_DIR_VER);
    addGestureCB(notifscr, LV_DIR_RIGHT, [](lv_event_t *e)
                 { setScreen(nullptr); });

    lv_obj_add_event_cb(notifscr, curvedScrollbarCB, LV_EVENT_SCROLL, createCurvedScrollbar(notifscr));

    title = lv_label_create(notifscr);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -80);
    lv_label_set_text(title, "Notification Title");

    body = lv_label_create(notifscr);
    lv_obj_align(body, LV_ALIGN_TOP_MID, 0, 60);
    lv_obj_set_width(body, 160);
    lv_label_set_text(body, "Notification Body");

    Notification_t temp;
    temp.title = "Test Notification Title";
    temp.body = "Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body Test Notification Body";
    showNotification(&temp);

    powermgmRegisterCB(notifperiodic, POWERMGM_LOOP_AWAKE, "notifscreeninit");
    return true;
}

bool notifsetup = powermgmRegisterCBPrio(notifcreate, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);