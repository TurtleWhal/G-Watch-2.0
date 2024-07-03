#include "Arduino.h"
#include "lvgl.h"

void setBacklight(int16_t);
void setBacklightGradual(int16_t, uint32_t);
int16_t getBacklight();
void setRotation(lv_display_rotation_t);