#include <Arduino.h>
#include "ArduinoLog.h"

#include "system.hpp"

#include "display.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "power.hpp"

SystemInfo sysinfo;

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  rtcInit();
  powerInit();

  // lv_init();

  displayInit();
  setBacklight(100);

  // lv_obj_t *scr = lv_obj_create(NULL);
  // lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  // lv_screen_load(scr);

  // lv_obj_t *btn = lv_button_create(scr);

  // lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

  // lv_obj_t *lbl = lv_label_create(btn);
  // lv_label_set_text(lbl, "Button!!!");

  // lv_obj_add_event_cb(btn, drag, LV_EVENT_PRESSING, nullptr);

  screenInit();
}

void loop()
{
  rtcPeriodic();
  powerPeriodic();
  screenPeriodic();
  delay(displayPeriodic());
}