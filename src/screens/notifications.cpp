#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "motor.hpp"
#include "notification.hpp"
#include "fonts/fonts.hpp"

lv_obj_t *notifsscr;
lv_obj_t *notifpanel;
int8_t notifx = 0, notify = -1;

lv_obj_t *createNotification(Notification_t *data)
{
    lv_obj_t *notif = lv_obj_create(notifsscr);
    lv_obj_set_width(notif, 180);
    lv_obj_set_style_height(notif, LV_SIZE_CONTENT, LV_PART_MAIN);
    lv_obj_set_style_radius(notif, 25, LV_PART_MAIN);

    lv_obj_set_scroll_dir(notif, LV_DIR_NONE);

    lv_obj_set_flex_flow(notif, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(notif, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *title = lv_label_create(notif);
    lv_label_set_text(title, data->title.c_str());
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_font(title, &Outfit_20, LV_PART_MAIN);
    lv_obj_set_width(title, 160);

    lv_obj_t *body = lv_label_create(notif);
    lv_label_set_text(body, data->body.c_str());
    lv_obj_align(body, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_width(body, 160);

    lv_obj_set_user_data(notif, data);

    return notif;
}

void drawNotifs()
{
    for (int i = lv_obj_get_child_count(notifsscr) - 1; i >= 0; i--) {
        if (lv_obj_get_child(notifsscr, i)->user_data != NULL) {
            lv_obj_del(lv_obj_get_child(notifsscr, i));
        }
    }

    forEachNotification([](Notification_t *notif)
                        { createNotification(notif); Serial.println(notif->title); });
}

bool notifsLoad(EventBits_t event, void *arg)
{

    if (ON_CURRENT_SCREEN(notifx, notify))
    {
        setScroll(LV_DIR_VER);
        drawNotifs();
    }

    return true;
}

void click(lv_event_t *e)
{
    motorVibrate(HAPTIC_NOTIFICATION);
}

bool notifsperiodic(EventBits_t event, void *arg)
{
    // if (ON_CURRENT_SCREEN(notifx, notify))
    // {
    //     Log.verboseln("on Notif screen");
    // }

    return true;
}

bool notifsscreate(EventBits_t event, void *arg)
{
    notifsscr = screenCreate(notifx, notify);

    // notifpanel = lv_obj_create(notifsscr);

    // lv_obj_set_size(notifpanel, 200, 180);
    // lv_obj_set_align(notifpanel, LV_ALIGN_CENTER);

    lv_obj_t *s1 = lv_obj_create(notifsscr);
    lv_obj_set_size(s1, 0, 80);

    lv_obj_set_flex_flow(notifsscr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(notifsscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_scroll_dir(notifsscr, LV_DIR_VER);
    createCurvedScrollbar(notifsscr);

    // lv_obj_t *notification = createNotification();
    // lv_obj_set_align(notification, LV_ALIGN_CENTER);

    // lv_obj_t *btn = lv_button_create(notifsscr);
    // lv_obj_set_size(btn, 180, 40);
    // lv_obj_set_style_radius(btn, 25, LV_PART_MAIN);
    // lv_obj_center(btn);
    // lv_obj_t *txt = lv_label_create(btn);
    // lv_obj_center(txt);
    // lv_label_set_text(txt, "Watch Go BRRRRR");

    // lv_obj_add_event_cb(btn, click, LV_EVENT_PRESSED, NULL);

    powermgmRegisterCB(notifsLoad, POWERMGM_SCREEN_CHANGE, "notifsscreenload");
    powermgmRegisterCB(notifsperiodic, POWERMGM_LOOP_AWAKE, "notifsscreeninit");
    return true;
}

bool notifssetup = powermgmRegisterCBPrio(notifsscreate, POWERMGM_INIT, "notifsscreeninit", CALL_CB_LAST);