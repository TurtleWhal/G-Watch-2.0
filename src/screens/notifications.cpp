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

lv_obj_t *nolabel;

lv_obj_t *createNotification(Notification_t *data)
{
    lv_obj_t *base = lv_obj_create(notifsscr);

    lv_obj_set_width(base, 180);
    lv_obj_set_height(base, 60);
    lv_obj_set_style_radius(base, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(base, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(base, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_set_scroll_dir(base, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(base, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(base, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(base, LV_FLEX_FLOW_ROW);

    lv_obj_t *space = lv_obj_create(base);
    lv_obj_set_width(space, 10);
    lv_obj_set_style_bg_opa(space, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(space, LV_OPA_TRANSP, LV_PART_MAIN);

    // lv_obj_t *arrow = lv_label_create(base);
    // SET_SYMBOL_32(arrow, FA_ARROW_RIGHT);

    lv_obj_t *notif = lv_obj_create(notifsscr);
    lv_obj_set_width(notif, 180);
    lv_obj_set_style_height(notif, LV_SIZE_CONTENT, LV_PART_MAIN);
    lv_obj_set_style_radius(notif, 25, LV_PART_MAIN);

    lv_obj_set_scroll_dir(notif, LV_DIR_NONE);

    lv_obj_set_flex_flow(notif, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(notif, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    if (data->title != "")
    {
        lv_obj_t *title = lv_label_create(notif);
        lv_label_set_text(title, data->title.c_str());
        lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_style_text_font(title, &Outfit_20, LV_PART_MAIN);
        lv_obj_set_width(title, 160);
    }

    if (data->body != "")
    {
        lv_obj_t *body = lv_label_create(notif);
        lv_label_set_text(body, data->body.c_str());
        lv_obj_align(body, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_width(body, 160);
    }

    lv_obj_add_event_cb(notif, [](lv_event_t *e)
                        { showNotification((Notification_t *)e->user_data); }, LV_EVENT_CLICKED, data);
    // lv_obj_add_event_cb(notif, [](lv_event_t *e)
    //                     {
    //                         if (lv_indev_get_gesture_dir(lv_indev_active()) == LV_DIR_RIGHT)
    //                         {
    //                             popNotificationId(((Notification_t *)e->user_data)->id);
    //                             drawNotifs();
    //                         }
    //                     },
    //                     LV_EVENT_GESTURE, data);

    lv_obj_set_user_data(base, data);
    lv_obj_scroll_to_view(notif, LV_ANIM_ON);

    return base;
}

bool notificationcheck;

void drawNotifs()
{
    for (int i = lv_obj_get_child_count(notifsscr) - 1; i >= 0; i--)
    {
        if (lv_obj_get_child(notifsscr, i)->user_data != NULL)
        {
            Log.verboseln("Deleting notification: %s", ((Notification_t *)lv_obj_get_child(notifsscr, i)->user_data)->title.c_str());
            lv_obj_delete(lv_obj_get_child(notifsscr, i));
        }
    }

    notificationcheck = true;
    forEachNotification([](Notification_t *notif)
                        { createNotification(notif); Serial.println(notif->title); notificationcheck = false; });

    if (notificationcheck)
        lv_obj_remove_flag(nolabel, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(nolabel, LV_OBJ_FLAG_HIDDEN);
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
    lv_obj_set_size(s1, 0, 40);

    lv_obj_set_flex_flow(notifsscr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(notifsscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_scroll_dir(notifsscr, LV_DIR_VER);
    createCurvedScrollbar(notifsscr);

    nolabel = lv_label_create(notifsscr);
    lv_label_set_text(nolabel, "No Notifications");
    lv_obj_align(nolabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(nolabel, &Outfit_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(nolabel, lv_color_darken(lv_color_white(), 40), LV_PART_MAIN);

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