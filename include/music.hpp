#include "Arduino.h"
#include "lvgl.h"

typedef struct
{
    String song;
    String artist;
    String album;
    uint16_t length;
    uint16_t position;
    bool playing;
} MusicInfo_t;

void updateMusicInfo(MusicInfo_t *);
void updateMusicState(MusicInfo_t *);
void musicInit();
void musicPeriodic();
MusicInfo_t *getMusicState();
void musicnext(lv_event_t *);
void musicprev(lv_event_t *);
void musicplaypause(lv_event_t *);