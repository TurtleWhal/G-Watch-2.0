#include "ArduinoLog.h"
#include "Arduino.h"
#include "ESP32Time.h"
#include "rtc.hpp"
#include "system.hpp"

#ifdef LILYGO_TWATCH_2021
#include "pcf8563.h"
PCF8563_Class pcf;
#endif // LILYGO_TWATCH_2021

ESP32Time rtc(0);

void rtcInit()
{
#ifdef LILYGO_TWATCH_2021
    pcf.begin();
    RTC_Date now = pcf.getDateTime();
    Log.verboseln("RTC: %d-%d-%d %d:%d:%d", now.year, now.month, now.day, now.hour, now.minute, now.second);
#else
    // rtc.setTime(56, 34, 12, 3, 12, 2008);
#endif // LILYGO_TWATCH_2021
}

void rtcPeriodic()
{
#ifdef LILYGO_TWATCH_2021
    static uint8_t lastmin = 0;
    RTC_Date now = pcf.getDateTime();

    if (now.minute != lastmin)
    {
        // pcf.syncToSystem();
        rtc.setTime(now.second, now.minute, now.hour, now.day, now.month, now.year);
        lastmin = now.minute;
    }
#endif // LILYGO_TWATCH_2021

    tm t_tm = rtc.getTimeStruct();

    sysinfo.time.hour12 = t_tm.tm_hour % 12 == 0 ? 12 : t_tm.tm_hour % 12;
    sysinfo.time.hour = t_tm.tm_hour;
    sysinfo.time.minute = t_tm.tm_min;
    sysinfo.time.second = t_tm.tm_sec;

    sysinfo.time.millis = rtc.getMillis();

    sysinfo.date.day = t_tm.tm_mday;
    sysinfo.date.month = t_tm.tm_mon + 1;
    sysinfo.date.year = t_tm.tm_year + 1900;

    String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    sysinfo.date.dow = t_tm.tm_wday;
    sysinfo.date.doy = t_tm.tm_yday;

    sysinfo.date.dayname = days[t_tm.tm_wday];
    sysinfo.date.monthname = months[t_tm.tm_mon];

    sysinfo.date.strdate = rtc.getTime("%a %b %e");
    sysinfo.date.strdate.replace("  ", " ");

    sysinfo.date.numdate = rtc.getTime("%D");
}

// timevalue is unix time in seconds, timezone is offset in hours
void setTime(ulong timevalue, short timezone)
{
    rtc.setTime(timevalue + (timezone * 60 * 60));
}