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
// lv_obj_t *notifsbox;
int8_t notifx = 0, notify = -1;

lv_obj_t *nolabel;
lv_obj_t *clrbtn;

lv_obj_t *displayNotification(Notification_t *data)
{
    lv_obj_t *base = lv_obj_create(notifsscr);
    // data->obj = base;

    lv_obj_set_width(base, 250);
    lv_obj_set_style_pad_ver(base, 0, LV_PART_MAIN);
    lv_obj_set_style_height(base, LV_SIZE_CONTENT, LV_PART_MAIN);
    lv_obj_set_style_radius(base, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(base, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(base, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_set_scroll_dir(base, LV_DIR_HOR);
    lv_obj_set_scroll_snap_x(base, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_scrollbar_mode(base, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(base, LV_FLEX_FLOW_ROW);

    lv_obj_t *space1 = lv_obj_create(base);
    lv_obj_set_size(space1, 160, 0);

    // lv_obj_t *arrow = lv_label_create(base);
    // SET_SYMBOL_32(arrow, FA_ARROW_RIGHT);

    lv_obj_t *notif = lv_obj_create(base);
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
        lv_obj_set_width(title, 120);
    }

    if (data->body != "")
    {
        lv_obj_t *body = lv_label_create(notif);
        lv_label_set_text(body, data->body.c_str());
        lv_obj_align(body, LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_width(body, 160);
    }

    lv_obj_t *space2 = lv_obj_create(base);
    lv_obj_set_size(space2, 160, 0);

    lv_obj_add_event_cb(notif, [](lv_event_t *e)
                        { showNotification((Notification_t *)e->user_data, false); }, LV_EVENT_CLICKED, data);
    lv_obj_add_event_cb(base, [](lv_event_t *e)
                        {
                            int scroll = lv_obj_get_scroll_x(lv_event_get_target_obj(e));
                            // Serial.println(scroll);

                            if (scroll == -30 || scroll == 326)
                            {
                                popNotificationId(((Notification_t *)e->user_data)->id);
                                lv_obj_delete(lv_event_get_target_obj(e));
                            } }, LV_EVENT_SCROLL_END, data);

    data->obj = base;
    lv_obj_set_user_data(base, data);
    lv_obj_scroll_to_view(notif, LV_ANIM_OFF);

    return base;
}

uint32_t checkid;
void deleteNotification(uint32_t id)
{
    // for (uint32_t i = 0; i < lv_obj_get_child_count(notifsscr); i++)
    // {
    //     void *data = lv_obj_get_child(notifsscr, i)->user_data;
    //     if (data != NULL && ((Notification_t *)data)->id == id)
    //     {
    //         Log.verboseln("Deleting notification: %s, type: %s", ((Notification_t *)data)->title.c_str(), lv_obj_get());
    //         // lv_obj_delete(((Notification_t *)data)->obj);
    //         lv_obj_set_style_bg_opa(((Notification_t *)data)->obj, LV_OPA_COVER, LV_PART_MAIN);
    //         lv_obj_set_style_bg_color(((Notification_t *)data)->obj, lv_color_hex(0xff0000), LV_PART_MAIN);
    //         // lv_obj_delete(lv_obj_get_child(notifsscr, i));
    //     }
    // }

    checkid = id;
    forEachNotification([](Notification_t *notif)
                        {
        if (notif->id == checkid)
        {
            Log.verboseln("Deleting notification: %s, With id: %i", notif->title.c_str(), notif->id);
            Log.verboseln(lv_obj_is_valid(notif->obj) ? "true" : "false");
            lv_obj_delete(notif->obj);
        } });
}

bool notificationcheck;

void drawNotifs()
{
    notificationcheck = false;

    forEachNotification([](Notification_t *notif)
                        { Serial.println(notif->title); notificationcheck = true; });

    if (notificationcheck)
    {
        lv_obj_add_flag(nolabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(clrbtn, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_remove_flag(nolabel, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(clrbtn, LV_OBJ_FLAG_HIDDEN);
    }
}

void clearNotifs()
{
    static uint8_t i = 0;
    forEachNotification([](Notification_t *notif)
                        {
                            // Log.verboseln("Clearing notification: %s", notif->title.c_str());
                            // deleteNotification(notif->id);
                            popNotification(i++);
                            lv_obj_delete(notif->obj);
                        });
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

    lv_obj_set_flex_flow(notifsscr, LV_FLEX_FLOW_COLUMN_REVERSE); // newer notifications on the top
    lv_obj_set_flex_align(notifsscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_scroll_dir(notifsscr, LV_DIR_VER);
    createCurvedScrollbar(notifsscr);

    nolabel = lv_label_create(notifsscr);
    lv_label_set_text(nolabel, "No Notifications");
    lv_obj_align(nolabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(nolabel, &Outfit_20, LV_PART_MAIN);
    lv_obj_set_style_text_color(nolabel, lv_color_darken(lv_color_white(), 40), LV_PART_MAIN);

    lv_obj_t *s1 = lv_obj_create(notifsscr);
    lv_obj_set_size(s1, 0, 20);

    clrbtn = lv_obj_create(notifsscr);
    lv_obj_align(clrbtn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(clrbtn, 180, 40);
    lv_obj_set_style_radius(clrbtn, 20, LV_PART_MAIN);
    lv_obj_set_scroll_dir(clrbtn, LV_DIR_NONE);

    lv_obj_t *lbl = lv_label_create(clrbtn);
    lv_obj_set_style_text_font(lbl, &Outfit_20, LV_PART_MAIN);
    lv_label_set_text(lbl, "Clear All");
    lv_obj_center(lbl);

    lv_obj_add_event_cb(clrbtn, [](lv_event_t *e)
                        { clearNotifs(); }, LV_EVENT_CLICKED, NULL);

    lv_obj_add_flag(clrbtn, LV_OBJ_FLAG_HIDDEN);

    // notifsbox = lv_obj_create(notifsscr);
    // lv_obj_set_style_bg_opa(notifsbox, LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_obj_set_style_border_opa(notifsbox, LV_OPA_TRANSP, LV_PART_MAIN);

    // lv_obj_set_width(notifsbox, 180);
    // lv_obj_set_style_height(notifsbox, LV_STYLE_MIN_HEIGHT, LV_PART_MAIN);

    // lv_obj_set_flex_flow(notifsbox, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_flex_align(notifsbox, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    powermgmRegisterCB(notifsLoad, POWERMGM_SCREEN_CHANGE, "notifsscreenload");
    powermgmRegisterCB(notifsperiodic, POWERMGM_LOOP_AWAKE, "notifsscreeninit");
    return true;
}

bool notifssetup = powermgmRegisterCBPrio(notifsscreate, POWERMGM_INIT, "notifsscreeninit", CALL_CB_LAST);