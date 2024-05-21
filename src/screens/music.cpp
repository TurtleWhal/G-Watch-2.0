#include "Arduino.h"
#include "lvgl.h"
#include "powermgm.hpp"
#include "system.hpp"
#include "screens.hpp"
#include "fonts/fonts.hpp"
#include "music.hpp"

lv_obj_t *musicscr;

lv_obj_t *playbutton;
lv_obj_t *nxtbutton;
lv_obj_t *prvbutton;
lv_obj_t *playlbl;

lv_obj_t *bar;

lv_obj_t *songlbl;
lv_obj_t *artistlbl;
lv_obj_t *albumlbl;
lv_obj_t *timelbl;

int8_t musicx = 0, musicy = 1;

bool musicUpdate(EventBits_t event, void *arg)
{

    MusicInfo_t *state = getMusicState();

    if (String(lv_label_get_text(songlbl)) != state->song)
        lv_label_set_text(songlbl, state->song.c_str());

    if (String(lv_label_get_text(artistlbl)) != state->artist)
        lv_label_set_text(artistlbl, state->artist.c_str());

    if (String(lv_label_get_text(albumlbl)) != state->album)
        lv_label_set_text(albumlbl, state->album.c_str());

    lv_bar_set_range(bar, 0, state->length);
    lv_bar_set_value(bar, state->position, LV_ANIM_ON);

    lv_label_set_text_fmt(timelbl, "%i:%02i / %i:%02i", state->position / 60, state->position % 60, state->length / 60, state->length % 60);

    if (state->playing)
    {
        SET_SYMBOL_40(playlbl, FA_PAUSE);
    }
    else
    {
        SET_SYMBOL_40(playlbl, FA_PLAY);
    }

    return true;
}

bool musicperiodic(EventBits_t event, void *arg)
{
    // if (ON_CURRENT_SCREEN(musicx, musicy))
    // {
    // }

    return true;
}

bool musiccreate(EventBits_t event, void *arg)
{
    musicscr = screenCreate(musicx, musicy);

    // Buttons
    playbutton = lv_btn_create(musicscr);
    lv_obj_set_size(playbutton, 70, 70);
    lv_obj_align(playbutton, LV_ALIGN_CENTER, 0, 80);
    lv_obj_set_style_radius(playbutton, 35, LV_PART_MAIN);
    lv_obj_set_style_bg_color(playbutton, lv_theme_get_color_primary(musicscr), LV_PART_MAIN);
    lv_obj_add_event_cb(playbutton, musicplaypause, LV_EVENT_CLICKED, NULL);

    playlbl = lv_label_create(playbutton);
    lv_obj_center(playlbl);
    SET_SYMBOL_40(playlbl, FA_PLAY);

    nxtbutton = lv_btn_create(musicscr);
    lv_obj_set_size(nxtbutton, 50, 50);
    lv_obj_align(nxtbutton, LV_ALIGN_CENTER, 70, 50);
    lv_obj_set_style_radius(nxtbutton, 25, LV_PART_MAIN);
    lv_obj_set_style_bg_color(nxtbutton, lv_theme_get_color_primary(musicscr), LV_PART_MAIN);
    lv_obj_add_event_cb(nxtbutton, musicnext, LV_EVENT_CLICKED, NULL);

    lv_obj_t *nextlbl = lv_label_create(nxtbutton);
    lv_obj_center(nextlbl);
    SET_SYMBOL_32(nextlbl, FA_STEP_FORWARD);

    prvbutton = lv_btn_create(musicscr);
    lv_obj_set_size(prvbutton, 50, 50);
    lv_obj_align(prvbutton, LV_ALIGN_CENTER, -70, 50);
    lv_obj_set_style_radius(prvbutton, 25, LV_PART_MAIN);
    lv_obj_set_style_bg_color(prvbutton, lv_theme_get_color_primary(musicscr), LV_PART_MAIN);
    lv_obj_add_event_cb(prvbutton, musicprev, LV_EVENT_CLICKED, NULL);

    lv_obj_t *prevlbl = lv_label_create(prvbutton);
    lv_label_set_text(prevlbl, LV_SYMBOL_PREV);
    lv_obj_center(prevlbl);
    SET_SYMBOL_32(prevlbl, FA_STEP_BACKWARD);

    // Slider
    bar = lv_bar_create(musicscr);
    lv_obj_align(bar, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_size(bar, 230, 5);

    // Labels
    songlbl = lv_label_create(musicscr);
    lv_obj_set_style_text_font(songlbl, &Outfit_32, 0);
    lv_label_set_long_mode(songlbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_set_width(songlbl, 220);
    lv_label_set_text(songlbl, "");
    lv_obj_align(songlbl, LV_ALIGN_LEFT_MID, -10, -24);

    artistlbl = lv_label_create(musicscr);
    lv_obj_align(artistlbl, LV_ALIGN_LEFT_MID, 0, -48);
    lv_label_set_text(artistlbl, "");

    timelbl = lv_label_create(musicscr);
    lv_obj_align(timelbl, LV_ALIGN_LEFT_MID, -15, 0);
    lv_label_set_text(timelbl, "0:00 / 0:00");
    // lv_obj_set_width(timelbl, 20);
    // lv_obj_set_style_text_align(timelbl, LV_TEXT_ALIGN_CENTER, 0);

    albumlbl = lv_label_create(musicscr);
    lv_label_set_long_mode(albumlbl, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(timelbl, 140);
    lv_label_set_text(albumlbl, "");
    lv_obj_align(albumlbl, LV_ALIGN_LEFT_MID, 80, 0);

    musicUpdate((EventBits_t)NULL, NULL);

    powermgmRegisterCB(musicperiodic, POWERMGM_LOOP_AWAKE, "MusicScrPeriodic");
    powermgmRegisterCB(musicUpdate, POWERMGM_MUSIC_UPDATE, "MusicScrUpdate");
    return true;
}

bool musicscrsetup = powermgmRegisterCBPrio(musiccreate, POWERMGM_INIT, "MusicScrCreate", CALL_CB_LAST);