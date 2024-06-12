#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "fonts/fonts.hpp"
#include "notification.hpp"
#include "ble.hpp"

lv_obj_t *notifscr;

lv_obj_t *icon;
lv_obj_t *title;
lv_obj_t *body;
lv_obj_t *reply;
lv_obj_t *replybtn;
lv_obj_t *replytext;
Notification_t notif;

lv_obj_t *scrollbar;

uint32_t notifmillis = 0;
bool notifshowing = false;

void showNotification(Notification_t *data)
{
    if (notifshowing)
        storeNotification(&notif);

    Log.verboseln("showing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d, Reply: %d", data->title.c_str(), data->body.c_str(), data->sender.c_str(), String(data->tel_number).c_str(), data->time, data->reply);

    SET_SYMBOL_40(icon, data->icon.c_str());
    lv_label_set_text(title, data->title.c_str());
    lv_label_set_text(body, data->body.c_str());

    setScreen(notifscr);

    lv_obj_scroll_to_y(notifscr, 0, LV_ANIM_ON);
    lv_obj_send_event(notifscr, LV_EVENT_SCROLL, scrollbar);

    lv_obj_add_event_cb(replybtn, [](lv_event_t *e)
                        {
                            String text = lv_textarea_get_text(replytext);
                            Notification_t *n = (Notification_t *)e->user_data;

                            if (text.length() > 0)
                            {
                                sendBLEf("{t:\"notify\", n:\"REPLY\", id:\"%i\", msg:\"%s\"}", n->id, text.c_str());
                            } }, LV_EVENT_PRESSED, data);

    if (data->reply)
        lv_obj_remove_flag(reply, LV_OBJ_FLAG_HIDDEN);
    else
        lv_obj_add_flag(reply, LV_OBJ_FLAG_HIDDEN);

    lv_textarea_set_text(replytext, "");

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

    if (lv_indev_get_state(lv_indev_active()) == LV_INDEV_STATE_PRESSED)
        notifmillis = millis();

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

    // addGestureCB(notifscr, LV_DIR_RIGHT, [](lv_event_t *e)
    //              { notifshowing = false; setScreen(nullptr); });

    // addGestureCB(notifscr, LV_DIR_LEFT, [](lv_event_t *e)
    //              { storeNotification(&notif); notifshowing = false; setScreen(nullptr); });

    lv_obj_add_event_cb(notifscr, [](lv_event_t *e)
                        {
                            lv_dir_t direction = lv_indev_get_gesture_dir(lv_indev_active());
                            if (direction == LV_DIR_LEFT)
                            {
                                storeNotification(&notif);
                                notifmillis = -1;
                                notifshowing = false;
                                setScreen(nullptr);
                            }
                            else if (direction == LV_DIR_RIGHT)
                            {
                                notifshowing = false;
                                setScreen(nullptr);
                            } }, LV_EVENT_GESTURE, nullptr);

    // lv_obj_add_event_cb(notifscr, [](lv_event_t *e)
    //                     { notifmillis = millis(); }, LV_EVENT_PRESSED, NULL);

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

    // reply box

    reply = lv_obj_create(notifscr);
    lv_obj_set_width(reply, 160);
    lv_obj_set_style_height(reply, LV_SIZE_CONTENT, LV_PART_MAIN);
    lv_obj_set_scroll_dir(reply, LV_DIR_NONE);

    lv_obj_t *replylbl = lv_label_create(reply);
    lv_label_set_text(replylbl, "Reply");
    lv_obj_align(replylbl, LV_ALIGN_TOP_LEFT, -10, -10);
    lv_obj_set_style_text_font(replylbl, &Outfit_20, LV_PART_MAIN);

    replybtn = lv_btn_create(reply);
    lv_obj_set_size(replybtn, 60, 20);
    lv_obj_align(replybtn, LV_ALIGN_TOP_RIGHT, 10, -10);

    lv_obj_t *replylbl2 = lv_label_create(replybtn);
    SET_SYMBOL_16(replylbl2, FA_SEND);
    lv_obj_center(replylbl2);

    replytext = lv_textarea_create(reply);
    lv_textarea_set_placeholder_text(replytext, "Type a reply...");
    lv_obj_align(replytext, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_width(replytext, 140);
    lv_obj_set_style_height(replytext, LV_SIZE_CONTENT, LV_PART_MAIN);

    attachKeyboard(replytext, KEYBOARD_CHARS_ALL);

    lv_obj_t *bs = lv_obj_create(notifscr);
    lv_obj_set_size(bs, 0, 20);

    powermgmRegisterCB(notifperiodic, POWERMGM_LOOP_AWAKE, "notifscreeninit");
    return true;
}

bool notifsetup = powermgmRegisterCBPrio(notifcreate, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);