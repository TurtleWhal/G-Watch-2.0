#include "ArduinoLog.h"
#include "notification.hpp"
#include "motor.hpp"
#include "powermgm.hpp"
#include "fonts/fonts.hpp"
#include <ArrayList.h>

ArrayList<Notification_t *> notifs(ArrayList<Notification_t *>::FIXED);

void storeNotification(Notification_t *notif)
{
    // Log.verboseln("storing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif->title.c_str(), notif->body.c_str(), notif->sender.c_str(), String(notif->tel_number).c_str(), notif->time);

    notifs.add(notif);

    forEachNotification([](Notification_t *notif) { Serial.println(notif->title); });
}

void forEachNotification(void (*func)(Notification_t *))
{
    notifs.forEach(func);
}

void handleNotification(String title, String subject, String body, String sender, String tel, String src, int id)
{
    powermgmTickle();
    motorVibrate(HAPTIC_NOTIFICATION);

    Log.verboseln("Recieved Notification, Title: %s, Subject: %s, Body: %s, Sender: %s, Tel: %s, Src: %s, Id: %d", title, subject, body, sender, tel, src, id);

    Notification_t *notif = new Notification_t();
    notif->title = (title != "") ? title : "Notification";
    notif->body = body;
    notif->sender = sender;
    // notif->tel_number = tel.toInt();
    notif->time = millis();
    notif->id = id;
    notif->src = src;

    if (src == "SMS Message")
        notif->icon = FA_SMS;
    else if (src == "Gadgetbridge")
        notif->icon = FA_GADGETBRIDGE;
    else if (src == "Messages")
        notif->icon = FA_MESSAGE;
    else if (src == "Youtube")
        notif->icon = FA_YOUTUBE;
    else if (src == "Discord")
        notif->icon = FA_DISCORD;
    else if (src == "Gmail")
        notif->icon = FA_EMAIL;
    else
        notif->icon = FA_BELL;

    showNotification(notif);
}