#include <Arduino.h>
#include "ArduinoLog.h"
#include <Wire.h>

#include "esp_pm.h"

#include "powermgm.hpp"
#include "system.hpp"

#include "lvgl.h"
#include "TFT_eSPI.h"

SystemInfo sysinfo;

void setup()
{
  delay(100);

  Serial.begin(115200);
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);

  esp_log_level_set("gpio", ESP_LOG_WARN);

  Wire.begin(IIC_SDA, IIC_SCL);

  //  _____/\\\\\\\\\\\\________________/\\\______________/\\\_____/\\\\\\\\\_____/\\\\\\\\\\\\\\\________/\\\\\\\\\__/\\\________/\\\_
  //   ___/\\\//////////________________\/\\\_____________\/\\\___/\\\\\\\\\\\\\__\///////\\\/////______/\\\////////__\/\\\_______\/\\\_
  //    __/\\\___________________________\/\\\_____________\/\\\__/\\\/////////\\\_______\/\\\_________/\\\/___________\/\\\_______\/\\\_
  //     _\/\\\____/\\\\\\\__/\\\\\\\\\\\_\//\\\____/\\\____/\\\__\/\\\_______\/\\\_______\/\\\________/\\\_____________\/\\\\\\\\\\\\\\\_
  //      _\/\\\___\/////\\\_\///////////___\//\\\__/\\\\\__/\\\___\/\\\\\\\\\\\\\\\_______\/\\\_______\/\\\_____________\/\\\/////////\\\_
  //       _\/\\\_______\/\\\_________________\//\\\/\\\/\\\/\\\____\/\\\/////////\\\_______\/\\\_______\//\\\____________\/\\\_______\/\\\_
  //        _\/\\\_______\/\\\__________________\//\\\\\\//\\\\\_____\/\\\_______\/\\\_______\/\\\________\///\\\__________\/\\\_______\/\\\_
  //         _\//\\\\\\\\\\\\/____________________\//\\\__\//\\\______\/\\\_______\/\\\_______\/\\\__________\////\\\\\\\\\_\/\\\_______\/\\\_
  //          __\////////////_______________________\///____\///_______\///________\///________\///______________\/////////__\///________\///__

  Log.verboseln("");
  Log.verboseln("");
  Log.verboseln("_____/\\\\\\\\\\\\\\\\\\\\\\\\________________/\\\\\\______________/\\\\\\_____/\\\\\\\\\\\\\\\\\\_____/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\________/\\\\\\\\\\\\\\\\\\__/\\\\\\________/\\\\\\_        ");
  Log.verboseln(" ___/\\\\\\//////////________________\\/\\\\\\_____________\\/\\\\\\___/\\\\\\\\\\\\\\\\\\\\\\\\\\__\\///////\\\\\\/////______/\\\\\\////////__\\/\\\\\\_______\\/\\\\\\_       ");
  Log.verboseln("  __/\\\\\\___________________________\\/\\\\\\_____________\\/\\\\\\__/\\\\\\/////////\\\\\\_______\\/\\\\\\_________/\\\\\\/___________\\/\\\\\\_______\\/\\\\\\_      ");
  Log.verboseln("   _\\/\\\\\\____/\\\\\\\\\\\\\\__/\\\\\\\\\\\\\\\\\\\\\\_\\//\\\\\\____/\\\\\\____/\\\\\\__\\/\\\\\\_______\\/\\\\\\_______\\/\\\\\\________/\\\\\\_____________\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_     ");
  Log.verboseln("    _\\/\\\\\\___\\/////\\\\\\_\\///////////___\\//\\\\\\__/\\\\\\\\\\__/\\\\\\___\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_______\\/\\\\\\_______\\/\\\\\\_____________\\/\\\\\\/////////\\\\\\_    ");
  Log.verboseln("     _\\/\\\\\\_______\\/\\\\\\_________________\\//\\\\\\/\\\\\\/\\\\\\/\\\\\\____\\/\\\\\\/////////\\\\\\_______\\/\\\\\\_______\\//\\\\\\____________\\/\\\\\\_______\\/\\\\\\_   ");
  Log.verboseln("      _\\/\\\\\\_______\\/\\\\\\__________________\\//\\\\\\\\\\\\//\\\\\\\\\\_____\\/\\\\\\_______\\/\\\\\\_______\\/\\\\\\________\\///\\\\\\__________\\/\\\\\\_______\\/\\\\\\_  ");
  Log.verboseln("       _\\//\\\\\\\\\\\\\\\\\\\\\\\\/____________________\\//\\\\\\__\\//\\\\\\______\\/\\\\\\_______\\/\\\\\\_______\\/\\\\\\__________\\////\\\\\\\\\\\\\\\\\\_\\/\\\\\\_______\\/\\\\\\_ ");
  Log.verboseln("        __\\////////////_______________________\\///____\\///_______\\///________\\///________\\///______________\\/////////__\\///________\\///__");
  Log.verboseln("");
  Log.verboseln("Ascii art made using: https://patorjk.com/software/taag/#p=display&f=Slant%20Relief&t=G-WATCH");
  Log.verboseln("Project made by: Garrett Jordan (https://github.com/TurtleWhal/G-Watch, https://garrettjordan.xyz/projects/#watch)");
  Log.verboseln("");
#ifdef WAVESHARE_ESP32_LCD
  Log.verboseln("Hardware: Waveshare ESP32-S3 LCD");
#elif LILYGO_TWATCH_2021
  Log.verboseln("Hardware: LilyGo T-Watch 2021");
#endif
  Log.verboseln("Device running ESP-IDF: %s, LVGL: %d.%d.%d, TFT_eSPI: %s", esp_get_idf_version(), lv_version_major(), lv_version_minor(), lv_version_patch(), TFT_ESPI_VERSION);
  Log.verboseln("Build Date: %s", __DATE__);
  Log.verboseln("");

  powermgmInit();
}

void loop()
{
  powermgmLoop();
}