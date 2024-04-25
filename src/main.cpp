#include <Arduino.h>
#include <Wire.h>
#include "display.hpp"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "CST816S.h"

extern TFT_eSPI tft;

extern CST816S touch;

static void drag(lv_event_t *event)
{
  lv_obj_set_x((lv_obj_t *)event->original_target, touch.data.x - 120);
  lv_obj_set_y((lv_obj_t *)event->original_target, touch.data.y - 120);
}

void setup()
{
  Serial.begin(115200);

  // lv_init();

  initDisplay();
  setBacklight(100);

  tft.fillScreen(TFT_RED);

  lv_obj_t *scr = lv_obj_create(NULL);
  lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

  lv_screen_load(scr);

  lv_obj_t *btn = lv_button_create(scr);

  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t *lbl = lv_label_create(btn);
  lv_label_set_text(lbl, "Button!!!");

  lv_obj_add_event_cb(btn, drag, LV_EVENT_PRESSING, nullptr);
}

void loop()
{
  delay(5);
  displayPeriodic();
}