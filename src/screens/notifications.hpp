#pragma GCC diagnostic ignored "-Wnarrowing"

#include "Arduino.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"

class NotificationsScreen
{
private:
    lv_obj_t * createNotification() {
        lv_obj_t *notif = lv_obj_create(scr);
        lv_obj_set_size(notif, 180, 40);
        lv_obj_set_style_radius(notif, 25, LV_PART_MAIN);

        return notif;
    }

public:
    void create(lv_obj_t *screen, uint8_t x, uint8_t y)
    {
        scr = screenCreate(x, y);

        lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        // lv_obj_t *notification = createNotification();
        // lv_obj_set_align(notification, LV_ALIGN_CENTER);
        createNotification();
        createNotification();
        createNotification();
        createNotification();
        createNotification();
    };

    void periodic()
    {
        
    };
    lv_obj_t *scr;
};