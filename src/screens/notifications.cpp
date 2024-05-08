#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"

lv_obj_t *notifscr;
lv_obj_t *notifpanel;
int8_t notifx = 0, notify = -1;

lv_obj_t *createNotification()
{
    lv_obj_t *notif = lv_obj_create(notifscr);
    lv_obj_set_size(notif, 180, 40);
    lv_obj_set_style_radius(notif, 25, LV_PART_MAIN);

    return notif;
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
    notifscr = screenCreate(notifx, notify);

    notifpanel = lv_obj_create(notifscr);

    lv_obj_set_size(notifpanel, 200, 180);
    lv_obj_set_align(notifpanel, LV_ALIGN_CENTER);

    // lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
    // lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // lv_obj_t *notification = createNotification();
    // lv_obj_set_align(notification, LV_ALIGN_CENTER);
    // createNotification();
    // createNotification();
    // createNotification();
    // createNotification();
    // createNotification();

    powermgmRegisterCB(notifperiodic, POWERMGM_LOOP, "notifscreeninit");
    return true;
}

bool notifsetup = powermgmRegisterCBPrio(notifcreate, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);