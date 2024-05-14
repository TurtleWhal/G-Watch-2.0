#include "Arduino.h"

typedef struct {
    String title;
    String body;
    String sender;
    uint64_t tel_number;
    uint32_t time;
} Notification_t;

void showNotification(Notification_t *);