#include "music.hpp"
#include "ble.hpp"
#include "powermgm.hpp"

MusicInfo_t *musicState;

bool musicPeriodic(EventBits_t event, void *arg)
{
    static uint32_t lastmillis;
    if (lastmillis + 1000 < millis())
    {
        if (musicState->playing)
        {
            powermgmSendEvent(POWERMGM_MUSIC_UPDATE);
            musicState->position++;
        }
        
        lastmillis = millis();
    }

    return true;
}

void updateMusicInfo(MusicInfo_t *info)
{
    musicState->song = info->song;
    musicState->artist = info->artist;
    musicState->album = info->album;
    musicState->length = info->length;

    powermgmSendEvent(POWERMGM_MUSIC_UPDATE);
}

void updateMusicState(MusicInfo_t *info)
{
    musicState->playing = info->playing;
    musicState->position = info->position;
    
    powermgmSendEvent(POWERMGM_MUSIC_UPDATE);
}

bool musicInit(EventBits_t event, void *arg)
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

    powermgmSendEvent(POWERMGM_MUSIC_UPDATE);

    powermgmRegisterCB(musicPeriodic, POWERMGM_LOOP, "MusicPeriodic");
    return true;
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
}

void musicnext(lv_event_t *e)
{
    sendBLE("{t:\"music\", n:\"next\"}", 1);
    // sendBLE("{t:\"music\", n:\"play\"}", 1);
}

void musicprev(lv_event_t *e)
{
    sendBLE("{t:\"music\", n:\"previous\"}", 1);
    // sendBLE("{t:\"music\", n:\"play\"}", 1);
}

bool musicsetup = powermgmRegisterCBPrio(musicInit, POWERMGM_INIT, "MusicInit", CALL_CB_LAST);