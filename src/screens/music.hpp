#include "Arduino.h"
#include "lvgl.h"
#include "system.hpp"
#include "system.hpp"
#include "screens.hpp"
#include "fonts/fonts.hpp"
#include "../include/music.hpp"

class MusicScreen
{
private:
    lv_obj_t *playbutton;
    lv_obj_t *nxtbutton;
    lv_obj_t *prvbutton;
    lv_obj_t *playlbl;

    lv_obj_t *bar;

    lv_obj_t *songlbl;
    lv_obj_t *artistlbl;
    lv_obj_t *albumlbl;
    lv_obj_t *timelbl;

public:
    void create(lv_obj_t *screen, uint8_t x, uint8_t y)
    {
        scr = screenCreate(x, y);

        // Buttons
        playbutton = lv_btn_create(scr);
        lv_obj_set_size(playbutton, 70, 70);
        lv_obj_align(playbutton, LV_ALIGN_CENTER, 0, 80);
        lv_obj_set_style_radius(playbutton, 35, LV_PART_MAIN);
        lv_obj_set_style_bg_color(playbutton, lv_theme_get_color_primary(scr), LV_PART_MAIN);
        lv_obj_add_event_cb(playbutton, musicplaypause, LV_EVENT_CLICKED, NULL);

        playlbl = lv_label_create(playbutton);
        lv_obj_center(playlbl);
        SET_SYMBOL_40(playlbl, FA_PLAY);

        nxtbutton = lv_btn_create(scr);
        lv_obj_set_size(nxtbutton, 50, 50);
        lv_obj_align(nxtbutton, LV_ALIGN_CENTER, 70, 50);
        lv_obj_set_style_radius(nxtbutton, 25, LV_PART_MAIN);
        lv_obj_set_style_bg_color(nxtbutton, lv_theme_get_color_primary(scr), LV_PART_MAIN);
        lv_obj_add_event_cb(nxtbutton, musicnext, LV_EVENT_CLICKED, NULL);

        lv_obj_t *nextlbl = lv_label_create(nxtbutton);
        lv_obj_set_style_text_font(playlbl, &FontAwesome_40, 0);
        lv_obj_center(nextlbl);
        SET_SYMBOL_20(nextlbl, FA_STEP_FORWARD);

        prvbutton = lv_btn_create(scr);
        lv_obj_set_size(prvbutton, 50, 50);
        lv_obj_align(prvbutton, LV_ALIGN_CENTER, -70, 50);
        lv_obj_set_style_radius(prvbutton, 25, LV_PART_MAIN);
        lv_obj_set_style_bg_color(prvbutton, lv_theme_get_color_primary(scr), LV_PART_MAIN);
        lv_obj_add_event_cb(prvbutton, musicprev, LV_EVENT_CLICKED, NULL);

        lv_obj_t *prevlbl = lv_label_create(prvbutton);
        lv_label_set_text(prevlbl, LV_SYMBOL_PREV);
        lv_obj_center(prevlbl);
        SET_SYMBOL_20(prevlbl, FA_STEP_BACKWARD);

        // Slider
        bar = lv_bar_create(scr);
        lv_obj_align(bar, LV_ALIGN_CENTER, 0, 14);
        lv_obj_set_size(bar, 210, 5);

        // Labels
        songlbl = lv_label_create(scr);
        lv_obj_set_style_text_font(songlbl, &Outfit_32, 0);
        lv_label_set_long_mode(songlbl, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(songlbl, 210);
        lv_label_set_text(songlbl, "Safe and Sound");
        lv_obj_align(songlbl, LV_ALIGN_LEFT_MID, 0, -24);

        artistlbl = lv_label_create(scr);
        lv_obj_align(artistlbl, LV_ALIGN_LEFT_MID, 0, -48);
        lv_label_set_text(artistlbl, "Capitol Cities");

        timelbl = lv_label_create(scr);
        lv_obj_align(timelbl, LV_ALIGN_LEFT_MID, 0, 0);
        lv_label_set_text(timelbl, "0:00 / 0:00");
        // lv_label_set_long_mode(albumlbl, LV_LABEL_LONG_CLIP);
        lv_obj_set_width(timelbl, 90);

        albumlbl = lv_label_create(scr);
        lv_label_set_long_mode(albumlbl, LV_LABEL_LONG_SCROLL);
        lv_obj_set_width(timelbl, 120);
        lv_label_set_text(albumlbl, "In a tidal wave of mystery");
        lv_obj_align(albumlbl, LV_ALIGN_LEFT_MID, 90, 0);
    };

    void periodic()
    {
        MusicInfo_t *state = getMusicState();

        lv_bar_set_range(bar, 0, state->length);
        lv_bar_set_value(bar, state->position, LV_ANIM_ON);

        lv_label_set_text(songlbl, state->song.c_str());
        lv_label_set_text(artistlbl, state->artist.c_str());
        lv_label_set_text(albumlbl, state->album.c_str());

        lv_label_set_text_fmt(timelbl, "%i:%02i / %i:%02i", state->position / 60, state->position % 60, state->length / 60, state->length % 60);

        if (state->playing)
        {
            SET_SYMBOL_40(playlbl, FA_PAUSE);
        }
        else
        {
            SET_SYMBOL_40(playlbl, FA_PLAY);
        }
    };

    lv_obj_t *scr;
};