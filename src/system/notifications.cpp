#include "ArduinoLog.h"
#include "notification.hpp"
#include "motor.hpp"
#include "powermgm.hpp"
#include "system.hpp"
#include "fonts/fonts.hpp"

#define NOTIF_COUNT 10

Notification_t nullnotif;
Notification_t notifs[NOTIF_COUNT];

uint32_t nowPlayingID;

void pushNotification(Notification_t *notif, uint8_t index = 0)
{
    Log.verboseln("Pushing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif->title.c_str(), notif->body.c_str(), notif->sender.c_str(), String(notif->tel_number).c_str(), notif->time);
    for (uint8_t i = NOTIF_COUNT - 1; i > index; i--)
    {
        notifs[i] = notifs[i - 1];
    }

    notifs[index] = *notif;
}

Notification_t popNotification(uint8_t index)
{
    Log.verboseln("Popping notification with index: %d", index);

    if (index == UINT8_MAX)
    {
        for (uint8_t i = 0; i < NOTIF_COUNT; i++)
        {
            if (notifs[i].id)
            {
                index = i;
                break;
            }
        }
    }

    Notification_t out = notifs[index];
    deleteNotification(out.id);

    for (uint8_t i = index; i < NOTIF_COUNT; i++)
    {
        notifs[i] = (i == NOTIF_COUNT - 1) ? nullnotif : notifs[i + 1];
    }

    drawNotifs();

    return out;
}

Notification_t popNotificationId(uint32_t id)
{
    if (nowPlayingID == id)
    {
        nowPlayingID = 0;
        sysinfo.glance.strings[sysinfo.glance.NOW_PLAYING] = "";
        return nullnotif;
    }
    for (uint8_t i = 0; i < NOTIF_COUNT; i++)
    {
        if (notifs[i].id == id)
        {
            return popNotification(i);
        }
    }

    return Notification_t();
}

void storeNotification(Notification_t *notif)
{
    Log.verboseln("storing notification with Name: %s, Body: %s, Sender: %s, Tel: %s, Time: %d", notif->title.c_str(), notif->body.c_str(), notif->sender.c_str(), String(notif->tel_number).c_str(), notif->time);

    displayNotification(notif);

    // forEachNotification([](Notification_t *n)
    //                     { Serial.println(n->title); });

    pushNotification(notif);

    drawNotifs();
}

void forEachNotification(void (*func)(Notification_t *), bool reversed)
{
    if (!reversed)
    {
        // Serial.print("ForEach: ");
        for (uint8_t i = 0; i < NOTIF_COUNT; i++)
        {
            // Serial.print(notifs[i].title);
            // Serial.print(", ");
            // Serial.print(notifs[i].id);
            // Serial.print(" | ");
            if (notifs[i].id)
                func(&notifs[i]);
        }
        // Serial.println();
    }
    else
    {
        // Serial.print("ForEach Reverse: ");
        for (uint8_t i = NOTIF_COUNT; i > 0; i--)
        {
            // Serial.print(notifs[i - 1].title);
            // Serial.print(", ");
            // Serial.print(notifs[i - 1].id);
            // Serial.print(" | ");
            if (notifs[i - 1].id)
                func(&notifs[i - 1]);
        }
        // Serial.println();
    }
}

void handleNotification(String title, String subject, String body, String sender, String tel, String src, int id, bool reply)
{
    Log.verboseln("Recieved Notification, Title: %s, Subject: %s, Body: %s, Sender: %s, Tel: %s, Src: %s, Id: %d", title.c_str(), subject.c_str(), body.c_str(), sender.c_str(), tel.c_str(), src.c_str(), id);

    if (strcmp(src.c_str(), "Android System Intelligence") == 0) // Check for Now Playing Notification
    {
        char nowPlayingTitle[64];
        char nowPlayingArtist[64];
        sscanf(title.c_str(), "%s by %s", nowPlayingTitle, nowPlayingArtist);

        char nowPlaying[132]; // 64 + 64 + " • " + null
        sprintf(nowPlaying, "%s • %s", nowPlayingTitle, nowPlayingArtist);
        sysinfo.glance.strings[sysinfo.glance.NOW_PLAYING] = nowPlaying;
        sysinfo.glance.icons[sysinfo.glance.NOW_PLAYING] = FA_MUSIC_NOTE;
        // Log.verboseln("Recieved Now Playing, \"%s\", id: %d", nowPlaying, nowPlayingID);
        Log.verboseln("Recieved Now Playing, Artist: %s, Song: %s, id: %d", nowPlayingArtist, nowPlayingTitle, nowPlayingID);

        nowPlayingID = id;
        return;
    }

    Notification_t *notif = new Notification_t();
    notif->title = (title != "") ? title : "Notification";
    notif->body = body;
    notif->sender = sender;
    // notif->tel_number = tel.toInt();
    notif->time = millis();
    notif->id = id;
    notif->src = src;
    notif->icon = FA_BELL;
    notif->reply = reply;

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

    showNotification(notif);

    if (!sysinfo.donotdisturb)
        motorVibrate(HAPTIC_NOTIFICATION);

    powermgmTickle();
}