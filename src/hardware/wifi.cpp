#include "ArduinoLog.h"
#include "WiFi.h"
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include "ArduinoOTA.h"
#include "wifi.hpp"
#include "powermgm.hpp"
#include "motor.hpp"

const char *ssid = "ThisNetworkIsOWN3D";
const char *password = "10244096";
// const char *ssid2 = "IT-Test";
// const char *password2 = "";

void wifiInit()
{
    WiFi.mode(WIFI_STA);
    WiFi.setHostname("ESP-Watch");
    WiFi.begin(ssid, password);

    // while (WiFi.waitForConnectResult() != WL_CONNECTED)
    // {
    //     Log.verboseln("Connection Failed! Rebooting...");
    //     delay(2000);
    //     WiFi.disconnect();
    //     WiFi.begin(ssid2, password2);
    //     delay(5000);
    //     ESP.restart();
    // }

    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Log.verbose("WiFi connected with IP address: ");
        Log.verboseln(WiFi.localIP());
    }

    // WiFi.onEvent([](WiFiEvent_t event)
    //              {
    //                      Log.verbose("WiFi connected with IP address: ");
    //                      Log.verboseln(WiFi.localIP()); },
    //              WIFI_EVENT_STA_CONNECTED);
}

void startOta()
{
    if (WiFi.status() != WL_CONNECTED)
        wifiInit();

    ArduinoOTA.onStart([]()
                       {
                           String type;
                           if (ArduinoOTA.getCommand() == U_FLASH)
                               type = "sketch";
                           else // U_SPIFFS
                               type = "filesystem";

                           // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                           Log.verboseln("Start updating %s", type);

                           //   _ui_screen_change(&ui_Alarm_Going_Off, LV_SCR_LOAD_ANIM_FADE_ON, 150, 0, &ui_Alarm_Going_Off_screen_init);
                           //   lv_label_set_text(ui_Alarm_Going_Off_Name, "OTA Uploading");
                           //   lv_obj_set_style_text_font(ui_Alarm_Going_Off_Name, &ui_font_Comfortaa_26, LV_PART_MAIN);
                           //   lv_label_set_text(ui_Alarm_Going_Off_Time, "Starting");
                           //   lv_obj_del(ui_Alarm_Going_Off_Stop_Button);
                           //   Wakeup("Ota Begin Upload");

                           powermgmTickle();

                           motorVibrate(HAPTIC_BUTTON); })
        .onEnd([]()
               { Log.verboseln("\nEnd"); })
        .onProgress([](unsigned int progress, unsigned int total)
                    {
                        uint8_t percent = (progress / (total / 100));
                        Log.verboseln("Progress: %u%%", percent);
                        //   lv_label_set_text_fmt(ui_Alarm_Going_Off_Time, "%i%%", percent);
                        powermgmTickle(); })
        .onError([](ota_error_t error)
                 {
                     Log.verboseln("Error[%u]: ", error);
                     if (error == OTA_AUTH_ERROR)
                         Log.verboseln("Auth Failed");
                     else if (error == OTA_BEGIN_ERROR)
                         Log.verboseln("Begin Failed");
                     else if (error == OTA_CONNECT_ERROR)
                         Log.verboseln("Connect Failed");
                     else if (error == OTA_RECEIVE_ERROR)
                         Log.verboseln("Receive Failed");
                     else if (error == OTA_END_ERROR)
                         Log.verboseln("End Failed");
                     //   ScreenBack(nullptr);
                 });

    ArduinoOTA.begin();

    // // info.OTA.useOTA = true;
    // // info.OTA.ip = WiFi.localIP().toString();
}