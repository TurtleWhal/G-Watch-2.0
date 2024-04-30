#include "ble.hpp"
#include "system.hpp"
#include <Arduino.h>
#include <ArduinoLog.h>

#include "ble/blectl.h"
#include "ble/gadgetbridge.h"

void parseGB(char *Message)
{
    Log.verboseln("Parsing Gadgetbridge message: %s", Message);
}

void parseBLE(char *Message)
{
    //   Log.verboseln(Message);
    Log.verboseln("Parsing BLE message: %s", Message);
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

void pairBT(uint32_t passkey)
{
    if (passkey < UINT32_MAX)
    {
        Log.verboseln("Pairing with BT: %u", passkey);
    }
    else
    {
        Log.verboseln("Paired BLE");
    }
}

void bleInit()
{
}

void blePeriodic()
{
}