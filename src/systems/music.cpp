#include "music.hpp"
#include "ble.hpp"

MusicInfo_t *musicState;
bool timerTriggered = false;
bool startedmoving = false;
bool wentforward = false;

void musicPeriodic()
{
    static int32_t last = millis();

    if (millis() - 1000 > last)
    {
        last = millis();
        if (musicState->playing)
            musicState->position++;
    }
}

void updateMusicInfo(MusicInfo_t *info)
{
    musicState->song = info->song;
    musicState->artist = info->artist;
    musicState->album = info->album;
    musicState->length = info->length;
};

void updateMusicState(MusicInfo_t *info)
{
    musicState->playing = info->playing;
    musicState->position = info->position + (info->playing ? 4 : 0); // if playing, add 4 seconds for send delay
}

void musicInit()
{
    musicState = new MusicInfo_t;

    // hw_timer_t *timer = NULL;
    // timer = timerBegin(1, 80, true);
    // timerAttachInterrupt(timer, []() { timerTriggered = true; }, true);
    // timerAlarmWrite(timer, 1000 * 1000, true);
    // timerAlarmEnable(timer);

    musicState->song = "No Song";
    musicState->artist = "No Artist";
    musicState->album = "No Album";
    musicState->length = 0;
    musicState->position = 0;
    musicState->playing = false;
}

MusicInfo_t *getMusicState()
{
    return musicState;
}

void musicplaypause(lv_event_t *e)
{
    if (getMusicState()->playing)
    {
        sendBLE("{t:\"music\", n:\"pause\"}", 2);
    }
    else
    {
        sendBLE("{t:\"music\", n:\"play\"}", 2);
    }
    musicState->playing = !musicState->playing;
};

void musicnext(lv_event_t *e)
{
    // uint8_t times = 1;
    // if (startedmoving)
    // {
    //     if (!wentforward)
    //         times = 3;

    //     wentforward = true;
    // }
    // else
    // {
    //     startedmoving = true;
    //     wentforward = true;
    // }
    sendBLE("{t:\"music\", n:\"next\"}", 1);
    sendBLE("{t:\"music\", n:\"play\"}", 1);
};

void musicprev(lv_event_t *e)
{
    // uint8_t times = 1;
    // if (startedmoving)
    // {
    //     if (wentforward)
    //         times = 3;

    //     wentforward = false;
    // }
    // else
    // {
    //     startedmoving = true;
    //     wentforward = false;
    // }
    sendBLE("{t:\"music\", n:\"previous\"}", 1);
    sendBLE("{t:\"music\", n:\"play\"}", 1);
};