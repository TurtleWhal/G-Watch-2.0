#include "Arduino.h"

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
void musicInit(void);
MusicInfo_t * getMusicState();