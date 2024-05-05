#include <Arduino.h>
#include "ArduinoLog.h"
#include <Wire.h>

#include "system.hpp"

#include "display.hpp"
#include "screen.hpp"
#include "rtc.hpp"
#include "power.hpp"
#include "music.hpp"

#ifndef DISABLE_BLE
#include "ble.hpp"
#endif // DISABLE_BLE

SystemInfo sysinfo;

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  esp_log_level_set("gpio", ESP_LOG_NONE);
  setCpuFrequencyMhz(240);

  Wire.begin(IIC_SDA, IIC_SCL);

  rtcInit();
  powerInit();

#ifndef DISABLE_BLE
  bleInit();
#endif // DISABLE_BLE

  musicInit();

  displayInit();
  setBacklight(100);

  screenInit();
}

void loop()
{
  rtcPeriodic();
  powerPeriodic();
#ifndef DISABLE_BLE
  blePeriodic();
#endif // DISABLE_BLE
  screenPeriodic();
  delay(displayPeriodic());

  // esp_sleep_enable_timer_wakeup(5 * 1000000); // microseconds
  // esp_light_sleep_start();
}