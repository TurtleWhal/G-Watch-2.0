#include "Arduino.h"
#include "lvgl.h"

typedef struct
{
    String title;
    String body;
    String sender;
    uint64_t tel_number;
    uint32_t time;
    uint32_t id;
    String src;
    String icon;
    bool reply;
    lv_obj_t *obj;
    // lv_img_dsc_t *img;
} Notification_t;

void handleNotification(String title, String subject, String body, String sender, String tel, String src, int id, bool reply = false);
void showNotification(Notification_t *, bool original = true);
void storeNotification(Notification_t *);
lv_obj_t *displayNotification(Notification_t *);
void deleteNotification(uint32_t id);
void forEachNotification(void (*func)(Notification_t *), bool = false);
void showAlert(String text, void (*cb)());
void drawNotifs();
Notification_t popNotification(uint8_t index = UINT8_MAX);
Notification_t popNotificationId(uint32_t id);