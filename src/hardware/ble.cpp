#include "ble.hpp"
#include "system.hpp"
#include <Arduino.h>
#include <ArduinoLog.h>

#include "ble/gadgetbridge.h"
#include "ble/blectl.h"
#include <NimBLEHIDDevice.h>

#define HID_SERVICE_UUID (uint16_t)0x1812
#define BATTERY_SERVICE_UUID (uint16_t)0x180F

// NimBLEHIDDevice *pHIDDevice;
// NimBLEService *pService;
NimBLEService *pBatteryService;
NimBLECharacteristic *pBatteryCharacteristic;
NimBLE2904* pBatteryDescriptor;

void parseGB(char *);

void bleInit()
{
    blectl_setup("Garrett's Watch");
    // setBLEBatteryLevel(100);
}

void blePeriodic()
{
    setBLEBatteryLevel(sysinfo.bat.percent);
}

void ble_setup()
{
    NimBLEServer *pServer = blectl_get_ble_server();
    NimBLEAdvertising *pAdvertising = blectl_get_ble_advertising();

    // pHIDDevice = new NimBLEHIDDevice(pServer);
    // pService = pServer->createService(NimBLEUUID((uint32_t)BATTERY_SERVICE_UUID));

    // pBatteryService = pHIDDevice->batteryService();
	pBatteryService = pServer->createService(NimBLEUUID((uint16_t) 0x180f));

	/*
	 * Mandatory battery level characteristic with notification and presence descriptor
	 */
	pBatteryCharacteristic = pBatteryService->createCharacteristic((uint16_t) 0x2a19, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
	pBatteryDescriptor = (NimBLE2904*)pBatteryCharacteristic->createDescriptor((uint16_t) 0x2904);
	pBatteryDescriptor->setFormat(NimBLE2904::FORMAT_UINT8);
	pBatteryDescriptor->setNamespace(1);
	pBatteryDescriptor->setUnit(0x27ad);

    pAdvertising->addServiceUUID(pBatteryService->getUUID());

    pBatteryService->start();
}

void pairBT(uint32_t passkey)
{
    if (passkey != UINT32_MAX)
    {
        Log.verboseln("Pairing with passkey: %d", passkey);
    }
    else
    {
        Log.verboseln("Paired BLE");
    }
}

void setBLEBatteryLevel(uint8_t level)
{
    // Serial.println(level);
    // pBatteryCharacteristic->setValue(&level, 1);
}

void parseBLE(char *Message)
{
    Log.verboseln(Message);
    struct timeval val;
    char *settime_str = NULL;
    ulong timevalue;
    short timezone;
    settime_str = strstr(Message, "setTime(");

    if (settime_str)
    {
        /* code */
        // Log.verboseln("GOT TIME!");
        settime_str = settime_str + 8;
        timevalue = atol(settime_str);
        // Log.verboseln(settime_str);
        Log.verboseln(settime_str);

        settime_str = strstr(Message, "setTimeZone(");
        settime_str = settime_str + 12;
        Log.verboseln(settime_str);
        timezone = atol(settime_str);

        Log.verboseln("%i", timezone);

        // SetTime(timevalue, timezone);
    }
    else
        parseGB(Message);
}

void parseGB(char *Message)
{
    // int info.music.length = 600;
    //  GB({t:"notify",id:1689704373,src:"Gadgetbridge",title:"",subject:"Testgh",body:"Testgh",sender:"Testgh",tel:"Testgh"})

    StaticJsonDocument<2048> received;

    // char Message2[] = "{t:\"notify\",id:1689704373,src:\"Gadgetbridge\",title:\"\",subject:\"Testgh\",body:\"Testgh\",sender:\"Testgh\",tel:\"Testgh\"}";
    // Message = "GB({t:\"notify\",id:1234567890,src:\"Messages\",title:\"Dad\",body:\"Test\"})";
    // GB({t:"notify",id:1234567890,src:"Messages",title:"Dad",body:"Test"})
    //  char json[] =
    //      "{sensor:\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
    const char *TempMessage = Message;
    // const char TempMessage[] = "GB({t:\"notify\",id:1689704373,src:\"Gadgetbridge\",title:\"Testgh\",subject:\"Testgh\",body:\"Testgh\",sender:\"Testgh\",tel:\"Testgh\"})";
    DeserializationError error = deserializeJson(received, TempMessage + 3); // message +3 to get rid of GB( at beginning of gadgetbridge message

    // Test if parsing succeeds.
    if (error)
    {
        // lv_label_set_text(ui_Now_Playing_Label, Message);
        return;
    }

    const char *NotifType = received["t"];

    //  if (strcmp(NotifType, "musicstate") != 0)
    // lv_label_set_text(ui_Now_Playing_Label, Message);

    // Log.verboseln(Message);

    //   if (strcmp(NotifType, "notify") == 0)
}