#include <Arduino.h>
#include "ArduinoLog.h"
#include <Wire.h>

#include "esp_pm.h"

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

#ifdef LILYGO_TWATCH_2021
esp_pm_config_esp32_t pm_config;
#else
esp_pm_config_esp32s3_t pm_config;
#endif
esp_pm_lock_handle_t lvgl_lock;

void setup()
{
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  esp_log_level_set("gpio", ESP_LOG_NONE);

  // pm_config.max_freq_mhz = 240;
  // pm_config.min_freq_mhz = 80;
  // pm_config.light_sleep_enable = true;
  // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
  // setCpuFrequencyMhz(240);

  Wire.begin(IIC_SDA, IIC_SCL);

  rtcInit();
  powerInit();

#ifndef DISABLE_BLE
  bleInit();
#endif // DISABLE_BLE

  musicInit();

  displayInit();
  setBacklight(100);
  // setBacklight(0);

  screenInit();

  // pm_config.max_freq_mhz = 240;
  // pm_config.min_freq_mhz = 80;
  // pm_config.light_sleep_enable = false;
  // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

  // esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, NULL, &lvgl_lock);
}

void loop()
{
  rtcPeriodic();
  powerPeriodic();
#ifndef DISABLE_BLE
  blePeriodic();
#endif // DISABLE_BLE

  // if (digitalRead(0))
  // {
  //   setBacklight(100);

  musicPeriodic();

  screenPeriodic();

  // esp_pm_lock_acquire(lvgl_lock);

  // pm_config.max_freq_mhz = 240;
  // pm_config.min_freq_mhz = 240;
  // pm_config.light_sleep_enable = false;
  // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
  uint32_t delaytime = displayPeriodic();

  // esp_pm_lock_release(lvgl_lock);

  delay(delaytime);
  // }
  // else
  // {
  //   setBacklight(0);
  //   delay(50);
  // }
}