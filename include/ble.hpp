#include <Arduino.h>

void ble_setup();
void bleInit();
void blePeriodic();
void parseBLE(char *);
void pairBT(uint32_t);
void setBLEBatteryLevel(uint8_t);