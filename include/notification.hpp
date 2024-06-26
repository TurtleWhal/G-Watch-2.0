#include "Arduino.h"

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
    // lv_img_dsc_t *img;
} Notification_t;

void handleNotification(String title, String subject, String body, String sender, String tel, String src, int id, bool reply = false);
void showNotification(Notification_t *);
void storeNotification(Notification_t *);
void forEachNotification(void (*func)(Notification_t *), bool = false);
void showAlert(String text, void (*cb)());
void drawNotifs();
Notification_t popNotificationId(uint32_t id);