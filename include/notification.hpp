#include "Arduino.h"
#include "ArrayList.h"

typedef struct
{
    String title;
    String body;
    String sender;
    uint64_t tel_number;
    uint32_t time;
} Notification_t;

void showNotification(Notification_t *);
void storeNotification(Notification_t *);
void forEachNotification(void (*)(Notification_t *));
void showAlert(String text, void (*cb)());