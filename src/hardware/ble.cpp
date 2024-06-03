#include "ble.hpp"
#include "rtc.hpp"
#include "system.hpp"
#include <Arduino.h>
#include <ArduinoLog.h>

#include "ble/gadgetbridge.h"
#include "ble/blectl.h"
#include <NimBLEHIDDevice.h>

#include "motor.hpp"
#include "music.hpp"
#include "screens/screens.hpp"
#include "powermgm.hpp"
#include "notification.hpp"

#define BLE_TERM_CHAR "\x1e" // 0x1e is "record seperator" in ascii and is used to seperate messages

#define HID_SERVICE_UUID (uint16_t)0x1812
#define BATTERY_SERVICE_UUID (uint16_t)0x180F

// NimBLEHIDDevice *pHIDDevice;
// NimBLEService *pService;
NimBLEService *pBatteryService;
NimBLECharacteristic *pBatteryCharacteristic;
NimBLE2904 *pBatteryDescriptor;

static String msg;
bool BLEtimer = false;

void parseGB(char *);
void BLEmsgloop();

void conn()
{
    Serial.println("BLE connected");
    sysinfo.ble.connected = true;
    powermgmSendEvent(POWERMGM_BLE_CONNECT);
}

void disconn()
{
    Serial.println("BLE disconnected");
    sysinfo.ble.connected = false;
    powermgmSendEvent(POWERMGM_BLE_DISCONNECT);
}

bool blePeriodic(EventBits_t event, void *arg)
{
    setBLEBatteryLevel(sysinfo.bat.percent);
    if (BLEtimer)
    {
        BLEtimer = false;
        BLEmsgloop();
    }

    return true;
}

bool bleInit(EventBits_t event, void *arg)
{
    blectl_setup("Garrett's Watch");
    // setBLEBatteryLevel(100);

    // BTmsgloop 50ms timer
    hw_timer_t *timer = NULL;
    timer = timerBegin(1, 80, true);
    timerAttachInterrupt(
        timer, []()
        { BLEtimer = true; },
        true);
    timerAlarmWrite(timer, BLECTL_CHUNKDELAY * 1000, true);
    timerAlarmEnable(timer);

    powermgmRegisterCB(blePeriodic, POWERMGM_LOOP, "blePeriodic");
    return true;
}

void ble_setup()
{
    NimBLEServer *pServer = blectl_get_ble_server();
    NimBLEAdvertising *pAdvertising = blectl_get_ble_advertising();

    // pHIDDevice = new NimBLEHIDDevice(pServer);
    // pService = pServer->createService(NimBLEUUID((uint32_t)BATTERY_SERVICE_UUID));

    // pBatteryService = pHIDDevice->batteryService();
    pBatteryService = pServer->createService(NimBLEUUID((uint16_t)0x180f));

    /*
     * Mandatory battery level characteristic with notification and presence descriptor
     */
    pBatteryCharacteristic = pBatteryService->createCharacteristic((uint16_t)0x2a19, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
    pBatteryDescriptor = (NimBLE2904 *)pBatteryCharacteristic->createDescriptor((uint16_t)0x2904);
    pBatteryDescriptor->setFormat(NimBLE2904::FORMAT_UINT8);
    pBatteryDescriptor->setNamespace(1);
    pBatteryDescriptor->setUnit(0x27ad);

    pAdvertising->addServiceUUID(pBatteryService->getUUID());

    pBatteryService->start();
}

void pairBT(uint32_t passkey)
{
    static lv_obj_t *pairscr;
    if (passkey != UINT32_MAX)
    {
        Log.verboseln("Pairing with passkey: %d", passkey);

        pairscr = lv_obj_create(NULL);
        lv_obj_t *pairlbl = lv_label_create(pairscr);
        lv_label_set_text_fmt(pairlbl, "Bluetooth Pairing code: \n%i", passkey);
        lv_obj_align(pairlbl, LV_ALIGN_CENTER, 0, 0);

        setScreen(pairscr, LV_SCR_LOAD_ANIM_FADE_IN);

        powermgmTickle();

        motorVibrate(HAPTIC_NOTIFICATION);
    }
    else
    {
        Log.verboseln("Paired BLE");

        setScreen(nullptr, LV_SCR_LOAD_ANIM_FADE_OUT);
        lv_obj_delete_delayed(pairscr, 2000);
    }
}

void setBLEBatteryLevel(uint8_t level)
{
    // Serial.println(level);
    // pBatteryCharacteristic->setValue(&level, 1);
}

void parseBLE(char *message)
{
    Log.verboseln("BLE message: %s", message);

    // struct timeval val;
    char *settime_str = NULL;
    ulong timevalue;
    short timezone;
    settime_str = strstr(message, "setTime(");

    if (settime_str)
    {
        settime_str = settime_str + 8;
        timevalue = atol(settime_str);

        settime_str = strstr(message, "setTimeZone(");
        settime_str = settime_str + 12;
        timezone = atol(settime_str);

        Log.verboseln("Time: %i, Timezone: %i", timevalue, timezone);

        setTime(timevalue, timezone);
    }
    else
    {
        parseGB(message);
    }
}

void parseGB(char *message)
{
    // int info.music.length = 600;
    //  GB({t:"notify",id:1689704373,src:"Gadgetbridge",title:"",subject:"Testgh",body:"Testgh",sender:"Testgh",tel:"Testgh"})

    // StaticJsonDocument<2048> received; // Depricated
    JsonDocument received;

    // char Message2[] = "{t:\"notify\",id:1689704373,src:\"Gadgetbridge\",title:\"\",subject:\"Testgh\",body:\"Testgh\",sender:\"Testgh\",tel:\"Testgh\"}";
    // message = "GB({t:\"notify\",id:1234567890,src:\"Messages\",title:\"Dad\",body:\"Test\"})";
    // GB({t:"notify",id:1234567890,src:"Messages",title:"Dad",body:"Test"})
    //  char json[] =
    //      "{sensor:\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    const char *tempMessage = message;
    // const char TempMessage[] = "GB({t:\"notify\",id:1689704373,src:\"Gadgetbridge\",title:\"Testgh\",subject:\"Testgh\",body:\"Testgh\",sender:\"Testgh\",tel:\"Testgh\"})";
    DeserializationError error = deserializeJson(received, tempMessage + 3); // message +3 to get rid of GB( at beginning of gadgetbridge message

    // Test if parsing succeeds.
    if (error)
    {
        // lv_label_set_text(ui_Now_Playing_Label, message);
        Log.verboseln("Error Parsing JSON: %s", error.c_str());
        return;
    }

    const char *notifType = received["t"];

    Log.verboseln("Notification Type: %s", notifType);

    //  if (strcmp(NotifType, "musicstate") != 0)
    // lv_label_set_text(ui_Now_Playing_Label, message);

    // Log.verboseln(message);

    if (strcmp(notifType, "is_gps_active") == 0)
    {
        sendBLE("{t:\"gps_power\", status: false}");
    }
    else if (strcmp(notifType, "reboot") == 0)
    {
        ESP.restart();
    }
    else if (strcmp(notifType, "musicinfo") == 0)
    {
        static MusicInfo_t musicInfo;

        musicInfo.song = received["track"].as<String>();
        musicInfo.artist = received["artist"].as<String>();
        musicInfo.album = received["album"].as<String>();
        musicInfo.length = received["dur"].as<uint16_t>();

        updateMusicInfo(&musicInfo);
    }
    else if (strcmp(notifType, "musicstate") == 0)
    {
        static MusicInfo_t musicState;

        // musicState.position = (int)received["position"];
        // musicState.playing = (strcmp((received["state"].as<char *>()), "play") == 0 ? true : false);
        musicState.position = received["position"].as<uint16_t>();
        musicState.playing = ((received["state"].as<String>() == "play") ? true : false);

        updateMusicState(&musicState);
    }
    else if (strcmp(notifType, "notify") == 0)
    {
        // powermgmTickle();

        // motorVibrate(HAPTIC_NOTIFICATION);

        // Notification_t notif;

        // if (received["title"].as<String>() != "")
        //     notif.title = received["title"].as<String>();
        // else
        //     notif.title = received["subject"].as<String>();

        // notif.body = received["body"].as<String>();
        // notif.sender = received["sender"].as<String>();

        // showNotification(&notif);

        handleNotification(
            received["title"].as<String>(),
            received["subject"].as<String>(),
            received["body"].as<String>(),
            received["sender"].as<String>(),
            received["tel"].as<String>(),
            received["src"].as<String>(),
            received["id"].as<int>());
    }
}

void sendBLE(String message, int repeat)
{
    for (int i = repeat; i > 0; i--)
    {
        msg = msg + "\r\n" + message + "\r\n" + BLE_TERM_CHAR;
        Log.verboseln("Sending BLE message: %s", message.c_str());
    }
}

void sendBLEf(const char *fmt, ...)
{
    char msg[256];

    va_list args;
    va_start(args, fmt);

    vsprintf(msg, fmt, args);

    va_end(args);

    sendBLE(msg);
}

bool isBLEConnected()
{
    return blectl_get_event(BLECTL_CONNECT | BLECTL_AUTHWAIT);
}

void BLEmsgloop()
{
    unsigned char tempmsg[BLECTL_CHUNKSIZE + 1];
    if (msg.length() and isBLEConnected())
    {
        if (msg.indexOf(BLE_TERM_CHAR) - 1 > BLECTL_CHUNKSIZE)
        {
            msg.getBytes(tempmsg + 1, BLECTL_CHUNKSIZE);
            msg.remove(0, BLECTL_CHUNKSIZE - 1);
            gadgetbridge_send_chunk(tempmsg, BLECTL_CHUNKSIZE);
        }
        else
        {
            msg.getBytes(tempmsg, BLECTL_CHUNKSIZE);
            gadgetbridge_send_chunk(tempmsg, msg.indexOf(BLE_TERM_CHAR) - 1);
            msg.remove(0, msg.indexOf(BLE_TERM_CHAR) + 1);
        }
        // Log.verboseln(msg);
    }
}

bool blesetup = powermgmRegisterCBPrio(bleInit, POWERMGM_INIT, "BLEInit", CALL_CB_FIRST);