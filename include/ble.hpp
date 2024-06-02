#include <Arduino.h>

void ble_setup();
void bleInit();
void blePeriodic();
void conn();
void disconn();
void parseBLE(char *);
void pairBT(uint32_t);
void setBLEBatteryLevel(uint8_t);
void sendBLE(String, int = 1);
void sendBLEf(const char *, ...);