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

lv_obj_t *scrollbar;

void showNotification(Notification_t *data)
{
    lv_label_set_text(title, data->title.c_str());
    lv_label_set_text(body, data->body.c_str());

    setScreen(notifscr);

    lv_obj_scroll_to_x(notifscr, 0, LV_ANIM_ON);
    lv_obj_send_event(notifscr, LV_EVENT_SCROLL, scrollbar);
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