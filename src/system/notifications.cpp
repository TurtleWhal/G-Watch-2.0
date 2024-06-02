#include "ArduinoLog.h"
#include "notification.hpp"
#include <ArrayList.h>

ArrayList<Notification_t *> notifs(ArrayList<Notification_t *>::FIXED);

void storeNotification(Notification_t *notif)
{
    Log.verboseln("storing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif->title.c_str(), notif->body.c_str(), notif->sender.c_str(), String(notif->tel_number).c_str(), notif->time);

    notifs.add(notif);
}

void forEachNotification(void (*func)(Notification_t *))
{
    notifs.forEach(func);
}