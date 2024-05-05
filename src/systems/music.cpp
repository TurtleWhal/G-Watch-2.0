#include "music.hpp"

MusicInfo_t *musicState;

void musicPeriodic()
{
    if (musicState->playing)
    {
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
    musicState->position = info->position;
}

void musicInit()
{
    musicState = new MusicInfo_t;

    hw_timer_t *timer = NULL;
    timer = timerBegin(1, 80, true);
    timerAttachInterrupt(timer, musicPeriodic, true);
    timerAlarmWrite(timer, 1000 * 1000, true);
    timerAlarmEnable(timer);
}

MusicInfo_t *getMusicState()
{
    return musicState;
}