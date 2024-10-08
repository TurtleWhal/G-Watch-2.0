#include "Arduino.h"

#ifndef SYSINFO
#define SYSINFO

typedef struct
// class SystemInfo
{
    // private:
    typedef struct
    {
        // 24 hour hour
        uint8_t hour = 12;
        // 12 hour hour
        uint8_t hour12 = 12;
        uint8_t minute = 0;
        uint8_t second = 0;
        // Milliseconds
        unsigned long millis = 0;
    } TimeInfo;
    TimeInfo time;

    typedef struct
    {
        // Formatted Mon Jan 1
        String strdate = "Mon Jan 1";
        // Formatted M/DD/YY
        String numdate = "1/1/70";

        uint8_t month = 1;
        // day of the month
        uint8_t day = 1;
        uint16_t year = 1970;

        // Day of Week (0-6)
        uint8_t dow = 0;
        // Day of Year (1-365 || 366)
        uint8_t doy = 1;

        // Jan, Feb, Mar...
        String monthname = "Jan";
        // Sun, Mon, Tue...
        String dayname = "Sun";
    } DateInfo;
    DateInfo date;

    typedef struct
    {
        // Battery Percentage
        uint8_t percent = 0;
        // Battery Voltage in mV
        float voltage = 0.0;
        // is charging
        bool charging = false;
    } BatteryInfo;
    BatteryInfo bat;

    typedef struct
    {
        uint32_t steps;

        typedef struct
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        } GyroInfo;
        GyroInfo gyro;

        typedef struct
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        } AccelInfo;
        AccelInfo acc;

    } IMUInfo;
    IMUInfo imu;

    typedef struct
    {
        uint32_t steps = 0;
        uint32_t goal = 10000;

    } HealthInfo;
    HealthInfo health;

    typedef struct
    {
        bool connected = false;
        String name = "New G-Watch";

    } BLEInfo;
    BLEInfo ble;

    typedef struct
    {
        bool connected = false;
    } WifiInfo;
    WifiInfo wifi;

    typedef struct
    {
        bool connected;
    } USBInfo;
    USBInfo usb;

    bool donotdisturb = false;
    bool sleeping = false;

    typedef struct
    {
        enum Codes
        {
            NOW_PLAYING,
            STOPWATCH,
            TIMER
        };

        int shown = 0;
        String strings[10];
        String icons[10];
    } GlanceInfo;
    GlanceInfo glance;

} SystemInfo;

#endif // SYSINFO

extern SystemInfo sysinfo;