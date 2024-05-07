#include "music.hpp"
#include "powermgm.hpp"

MusicInfo_t *musicState;
bool timerTriggered = false;

bool musicPeriodic(EventBits_t event, void *arg)
{
    if (timerTriggered && musicState->playing)
    {
        musicState->position++;
    }

    return true;
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
    musicState->position = info->position;
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

    powermgmRegisterCB(musicPeriodic, POWERMGM_LOOP, "MusicPeriodic");
    return true;
}

MusicInfo_t *getMusicState()
{
    return musicState;
}

bool musicsetup = powermgmRegisterCBPrio(musicInit, POWERMGM_INIT, "MusicInit", CALL_CB_LAST);