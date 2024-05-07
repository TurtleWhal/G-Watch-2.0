#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"

lv_obj_t *mainscr;
lv_obj_t *notifscr;
lv_obj_t *notifpanel;
int8_t x = 0, y = -1;

lv_obj_t *createNotification()
{
    lv_obj_t *notif = lv_obj_create(notifscr);
    lv_obj_set_size(notif, 180, 40);
    lv_obj_set_style_radius(notif, 25, LV_PART_MAIN);

    return notif;
}

bool periodic(EventBits_t event, void *arg)
{
    if (lv_obj_get_scroll_x(mainscr) == x * TFT_WIDTH && lv_obj_get_scroll_y(mainscr) == y * TFT_HEIGHT)
    {
        Log.verboseln("on Notif screen");
    }

    return true;
};

bool create(EventBits_t event, void *arg)
{
    mainscr = lv_scr_act();
    notifscr = screenCreate(x, y);

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

    powermgmRegisterCB(periodic, POWERMGM_LOOP, "notifscreeninit");
    return true;
};

bool notifsetup = powermgmRegisterCBPrio(create, POWERMGM_INIT, "notifscreeninit", CALL_CB_LAST);