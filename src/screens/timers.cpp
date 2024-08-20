#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "fonts/fonts.hpp"
#include "powermgm.hpp"
#include "notification.hpp"
#include "motor.hpp"

lv_obj_t *timerscr;
lv_obj_t *timerscrollbar;

lv_obj_t *stopwatch;
lv_obj_t *timer;
lv_obj_t *alarms;

uint32_t stopwatchmillis = 0;
bool stopwatchrunning = false;

lv_obj_t *sSroller;
lv_obj_t *Ssroller;
lv_obj_t *mMroller;
lv_obj_t *Mmroller;
lv_obj_t *hHroller;
lv_obj_t *Hhroller;

ulong timermillis = 0;
bool timerrunning = false;

lv_obj_t *stoplbl;
lv_obj_t *timerplaylbl;

int8_t timerx = 1, timery = 0;

void writeTimerTime(uint32_t millis)
{
    // Log.verboseln("Writing Timertime: %d", millis);
    lv_roller_set_selected(sSroller, (millis / 1000) % 10, LV_ANIM_ON);
    lv_roller_set_selected(Ssroller, ((millis / 1000) % 60) / 10, LV_ANIM_ON);
    lv_roller_set_selected(mMroller, (millis / 60000) % 10, LV_ANIM_ON);
    lv_roller_set_selected(Mmroller, ((millis / 60000) % 60) / 10, LV_ANIM_ON);
    lv_roller_set_selected(hHroller, (millis / 3600000) % 10, LV_ANIM_ON);
    lv_roller_set_selected(Hhroller, ((millis / 3600000) % 99) / 10, LV_ANIM_ON);
}

void setTimerTime(lv_event_t *e)
{
    timermillis = 0;
    timermillis += lv_roller_get_selected(sSroller) * 1000;
    timermillis += lv_roller_get_selected(Ssroller) * 10000;
    timermillis += lv_roller_get_selected(mMroller) * 60000;
    timermillis += lv_roller_get_selected(Mmroller) * 600000;
    timermillis += lv_roller_get_selected(hHroller) * 3600000;
    timermillis += lv_roller_get_selected(Hhroller) * 36000000;
    // Log.verboseln("Timertime: %d", timermillis);
}

bool timerLoad(EventBits_t event, void *arg)
{
    setScroll(LV_DIR_HOR);
    return true;
}

bool timerperiodic(EventBits_t event, void *arg)
{
    static uint32_t lastmillis = 0;
    static uint32_t secmillis = 0;
    bool secchanged = false;

    uint32_t diff = millis() - lastmillis;
    lastmillis = millis();

    secmillis += diff;

    if (secmillis > 1000)
    {
        secchanged = true;
        secmillis = 0;
    }

    if (stopwatchrunning)
    {
        stopwatchmillis += diff;

        char stop[8];
        if (timermillis >= 3600000)
            sprintf(stop, "%i:%02i", (uint8_t)(stopwatchmillis / 3600000), (uint8_t)((stopwatchmillis % 3600000) / 60000));
        else
            sprintf(stop, "%i:%02i", (uint8_t)((stopwatchmillis % 3600000) / 60000), (uint8_t)((stopwatchmillis % 60000) / 1000));
        sysinfo.glance.strings[sysinfo.glance.STOPWATCH] = stop;
        sysinfo.glance.icons[sysinfo.glance.STOPWATCH] = FA_STOPWATCH;
    }
    else
    {
        sysinfo.glance.strings[sysinfo.glance.STOPWATCH] = "";
    }

    if (timerrunning)
    {
        timermillis -= diff;

        char timer[8];
        if (timermillis >= 3600000)
            sprintf(timer, "%i:%02i", (uint8_t)(timermillis / 3600000), (uint8_t)((timermillis % 3600000) / 60000));
        else
            sprintf(timer, "%i:%02i", (uint8_t)((timermillis % 3600000) / 60000), (uint8_t)((timermillis % 60000) / 1000));
        sysinfo.glance.strings[sysinfo.glance.TIMER] = timer;
        sysinfo.glance.icons[sysinfo.glance.TIMER] = FA_TIMER;
    }
    else
    {
        sysinfo.glance.strings[sysinfo.glance.TIMER] = "";
    }

    if (ON_CURRENT_SCREEN(timerx, timery))
    {
        if (stopwatchrunning)
        {
            if (stopwatchmillis >= 3600000)
                lv_label_set_text_fmt(stoplbl, "%02d:%02d:%02d", stopwatchmillis / 3600000, (stopwatchmillis % 3600000) / 60000, (stopwatchmillis % 60000) / 1000);
            else
                lv_label_set_text_fmt(stoplbl, "%02d:%02d:%02d", (stopwatchmillis % 3600000) / 60000, (stopwatchmillis % 60000) / 1000, (stopwatchmillis % 1000) / 10);
        }

        if (timerrunning)
        {
            if (timermillis > 0 && secchanged)
            {
                writeTimerTime(timermillis);
            }
        }
    }
    
    if (timerrunning && timermillis > ULONG_MAX - 1000)
    {
        powermgmTickle();
        writeTimerTime(0);
        timerrunning = false;
        motorVibrate(30, 1000);
        SET_SYMBOL_32(timerplaylbl, FA_PLAY);

        showAlert("Alarm", []()
                  { motorVibrate(0, 0); });
    }

    return true;
}

bool timercreate(EventBits_t event, void *arg)
{
    timerscr = screenCreate(timerx, timery);
    lv_obj_set_scroll_dir(timerscr, LV_DIR_VER);

    lv_obj_set_scroll_snap_y(timerscr, LV_SCROLL_SNAP_CENTER);

    timerscrollbar = createCurvedScrollbar(timerscr);

    // Stopwatch Screen
    stopwatch = lv_obj_create(timerscr);
    lv_obj_set_size(stopwatch, 240, 240);
    lv_obj_align(stopwatch, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(stopwatch, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(stopwatch, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_scroll_dir(stopwatch, LV_DIR_NONE);

    stoplbl = lv_label_create(stopwatch);
    lv_label_set_text(stoplbl, "00:00:00");
    lv_obj_set_style_text_font(stoplbl, &Outfit_46, 0);
    lv_obj_align(stoplbl, LV_ALIGN_LEFT_MID, -4, -10);

    lv_obj_t *stopplay = lv_button_create(stopwatch);
    lv_obj_set_size(stopplay, 100, 50);
    lv_obj_align(stopplay, LV_ALIGN_CENTER, -35, 60);
    lv_obj_set_style_radius(stopplay, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(stopplay, lv_theme_get_color_primary(timerscr), LV_PART_MAIN);

    lv_obj_t *playlbl = lv_label_create(stopplay);
    SET_SYMBOL_32(playlbl, FA_PLAY);
    lv_obj_center(playlbl);

    lv_obj_t *rstbtn = lv_button_create(stopwatch);
    lv_obj_set_size(rstbtn, 50, 50);
    lv_obj_align(rstbtn, LV_ALIGN_CENTER, 50, 60);
    lv_obj_set_style_radius(rstbtn, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(rstbtn, lv_theme_get_color_primary(timerscr), LV_PART_MAIN);

    lv_obj_t *rstlbl = lv_label_create(rstbtn);
    SET_SYMBOL_32(rstlbl, FA_RESET);
    lv_obj_center(rstlbl);

    lv_obj_add_event_cb(stopplay, [](lv_event_t *e)
                        { stopwatchrunning = !stopwatchrunning; if (stopwatchrunning) {SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PAUSE);} else {SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PLAY);} }, LV_EVENT_CLICKED, playlbl);
    lv_obj_add_event_cb(rstbtn, [](lv_event_t *e)
                        { stopwatchrunning = false; SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PLAY); stopwatchmillis = 0; }, LV_EVENT_CLICKED, playlbl);
    lv_obj_add_event_cb(rstbtn, [](lv_event_t *e)
                        { lv_label_set_text((lv_obj_t *)e->user_data, "00:00:00"); }, LV_EVENT_CLICKED, stoplbl);
    lv_obj_add_event_cb(stopplay, [](lv_event_t *e)
                        { motorVibrate(HAPTIC_BUTTON); }, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(rstbtn, [](lv_event_t *e)
                        { motorVibrate(HAPTIC_BUTTON); }, LV_EVENT_PRESSED, NULL);

    // Timer Screen
    timer = lv_obj_create(timerscr);
    lv_obj_set_size(timer, 240, 240);
    lv_obj_align(timer, LV_ALIGN_CENTER, 0, 240);
    lv_obj_set_style_bg_opa(timer, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(timer, LV_OPA_TRANSP, LV_PART_MAIN);

    sSroller = lv_roller_create(timer);
    Ssroller = lv_roller_create(timer);
    mMroller = lv_roller_create(timer);
    Mmroller = lv_roller_create(timer);
    hHroller = lv_roller_create(timer);
    Hhroller = lv_roller_create(timer);
    lv_obj_t *cC = lv_label_create(timer);
    lv_obj_t *Cc = lv_label_create(timer);

    lv_obj_set_size(sSroller, 36, 60);
    lv_obj_set_size(Ssroller, 36, 60);
    lv_obj_set_size(mMroller, 36, 60);
    lv_obj_set_size(Mmroller, 36, 60);
    lv_obj_set_size(hHroller, 36, 60);
    lv_obj_set_size(Hhroller, 36, 60);

    lv_obj_align(sSroller, LV_ALIGN_CENTER, 75, 0);
    lv_obj_align(Ssroller, LV_ALIGN_CENTER, 47, 0);
    lv_obj_align(mMroller, LV_ALIGN_CENTER, 14, 0);
    lv_obj_align(Mmroller, LV_ALIGN_CENTER, -14, 0);
    lv_obj_align(hHroller, LV_ALIGN_CENTER, -47, 0);
    lv_obj_align(Hhroller, LV_ALIGN_CENTER, -75, 0);

    lv_obj_align(cC, LV_ALIGN_CENTER, 30, 0);
    lv_obj_align(Cc, LV_ALIGN_CENTER, -30, 0);

    lv_obj_set_style_radius(sSroller, 10, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(Hhroller, 10, LV_PART_SELECTED | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_opa(sSroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Ssroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(mMroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Mmroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(hHroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(Hhroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_border_opa(sSroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Ssroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(mMroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Mmroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(hHroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(Hhroller, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(sSroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Ssroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(mMroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Mmroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(hHroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Hhroller, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(cC, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(Cc, &Outfit_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_roller_set_options(sSroller, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_options(Ssroller, "0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_options(mMroller, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_options(Mmroller, "0\n1\n2\n3\n4\n5", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_options(hHroller, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);
    lv_roller_set_options(Hhroller, "0\n1\n2\n3\n4\n5\n6\n7\n8\n9", LV_ROLLER_MODE_INFINITE);

    lv_label_set_text(cC, ":");
    lv_label_set_text(Cc, ":");

    lv_obj_add_event_cb(sSroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(Ssroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(mMroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(Mmroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(hHroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(Hhroller, setTimerTime, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *timerplay = lv_button_create(timer);
    lv_obj_set_size(timerplay, 100, 50);
    lv_obj_align(timerplay, LV_ALIGN_CENTER, -35, 60);
    lv_obj_set_style_radius(timerplay, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timerplay, lv_theme_get_color_primary(timerscr), LV_PART_MAIN);

    timerplaylbl = lv_label_create(timerplay);
    SET_SYMBOL_32(timerplaylbl, FA_PLAY);
    lv_obj_center(timerplaylbl);

    lv_obj_t *timerrst = lv_button_create(timer);
    lv_obj_set_size(timerrst, 50, 50);
    lv_obj_align(timerrst, LV_ALIGN_CENTER, 50, 60);
    lv_obj_set_style_radius(timerrst, 40, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timerrst, lv_theme_get_color_primary(timerscr), LV_PART_MAIN);

    lv_obj_t *timerrstlbl = lv_label_create(timerrst);
    SET_SYMBOL_32(timerrstlbl, FA_RESET);
    lv_obj_center(timerrstlbl);

    lv_obj_add_event_cb(timerplay, [](lv_event_t *e)
                        { timerrunning = !timerrunning; if (timerrunning) {SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PAUSE);} else {SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PLAY);} }, LV_EVENT_CLICKED, timerplaylbl);
    lv_obj_add_event_cb(timerrst, [](lv_event_t *e)
                        { timerrunning = false; SET_SYMBOL_32((lv_obj_t *)e->user_data, FA_PLAY); timermillis = 0; writeTimerTime(timermillis); }, LV_EVENT_CLICKED, timerplaylbl);
    lv_obj_add_event_cb(timerplay, [](lv_event_t *e)
                        { motorVibrate(HAPTIC_BUTTON); }, LV_EVENT_PRESSED, NULL);
    lv_obj_add_event_cb(timerrst, [](lv_event_t *e)
                        { motorVibrate(HAPTIC_BUTTON); }, LV_EVENT_PRESSED, NULL);

    // Alarms Screen
    alarms = lv_obj_create(timerscr);
    lv_obj_set_size(alarms, 240, 240);
    lv_obj_align(alarms, LV_ALIGN_CENTER, 0, 480);
    lv_obj_set_style_bg_opa(alarms, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(alarms, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_t *lbl3 = lv_label_create(alarms);
    lv_label_set_text(lbl3, "Alarms\nComing\nSoon™");
    lv_obj_set_style_text_font(lbl3, &Outfit_32, LV_PART_MAIN);
    lv_obj_set_align(lbl3, LV_ALIGN_CENTER);
    lv_obj_set_style_text_align(lbl3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    powermgmRegisterCB(timerLoad, POWERMGM_SCREEN_CHANGE, "TimerLoad");
    powermgmRegisterCB(timerperiodic, POWERMGM_LOOP, "timerscreeninit");
    return true;
}

bool timersetup = powermgmRegisterCBPrio(timercreate, POWERMGM_INIT, "timerscreeninit", CALL_CB_LAST);