#include "ArduinoLog.h"
#include "notification.hpp"
#include "motor.hpp"
#include "powermgm.hpp"
#include "system.hpp"
#include "fonts/fonts.hpp"

Notification_t *notifs[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

void pushNotification(Notification_t *notif, uint8_t index = 0)
{
    for (uint8_t i = 9; i > index; i--)
    {
        notifs[i] = notifs[i - 1];
    }

    notifs[index] = notif;
}

Notification_t *popNotification(uint8_t index = UINT8_MAX)
{
    if (index == UINT8_MAX)
    {
        for (uint8_t i = 0; i < 9; i++)
        {
            if (notifs[i] == nullptr)
            {
                index = i;
                break;
            }
        }
    }

    Notification_t *out = notifs[index];

    for (uint8_t i = index; i < 9; i++)
    {
        notifs[i] = notifs[i + 1];
    }

    return out;
}

void storeNotification(Notification_t *notif)
{
    // Log.verboseln("storing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif->title.c_str(), notif->body.c_str(), notif->sender.c_str(), String(notif->tel_number).c_str(), notif->time);

    pushNotification(notif);

    forEachNotification([](Notification_t *n)
                        { Serial.println(n->title); });
}

void forEachNotification(void (*func)(Notification_t *))
{
    for (uint8_t i = 0; i < 10; i++)
    {
        if (notifs[i] != nullptr)
            Serial.println(notifs[i]->title);
        // func(notifs[i]);
    }
}

void handleNotification(String title, String subject, String body, String sender, String tel, String src, int id)
{
    powermgmTickle();

    if (!sysinfo.donotdisturb)
    motorVibrate(HAPTIC_NOTIFICATION);

    Log.verboseln("Recieved Notification, Title: %s, Subject: %s, Body: %s, Sender: %s, Tel: %s, Src: %s, Id: %d", title.c_str(), subject.c_str(), body.c_str(), sender.c_str(), tel.c_str(), src.c_str(), id);

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