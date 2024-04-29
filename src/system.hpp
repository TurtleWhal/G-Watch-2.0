#include "Arduino.h"

typedef struct
// class SystemInfo
{
    // private:
    typedef struct
    {
        // 24 hour hour
        uint8_t hour;
        // 12 hour hour
        uint8_t hour12;
        uint8_t minute;
        uint8_t second;
        // Milliseconds
        unsigned long millis;
    } TimeInfo;
    TimeInfo time;

    typedef struct
    {
        // Formatted Mon Jan 1
        String strdate;
        // Formatted M/DD/YY
        String numdate;

        uint8_t month;
        // day of the month
        uint8_t day;
        uint8_t year;

        // Day of Week (0-6)
        uint8_t dow;
        // Day of Year (1-365 || 366)
        uint8_t doy;

        // Jan, Feb, Mar...
        String monthname;
        // Sun, Mon, Tue...
        String dayname;
    } DateInfo;
    DateInfo date;

    typedef struct {
        // Battery Percentage
        uint8_t percent;
        // Battery Voltage in mV
        float voltage;
        // is charging
        bool charging;
    } BatteryInfo;
    BatteryInfo bat;

} SystemInfo;

extern SystemInfo sysinfo;