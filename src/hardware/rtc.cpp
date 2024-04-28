#include "ArduinoLog.h"
#include "Arduino.h"
#include "pcf8563.h"
#include "rtc.hpp"
#include "system.hpp"

PCF8563_Class rtc;

void rtcInit()
{
    rtc.begin(Wire);
}

void rtcPeriodic()
{
    static uint8_t lastmin = 0;
    RTC_Date now = rtc.getDateTime();

    timeval val;
    struct tm t_tm;

    if (now.minute != lastmin)
    {
        rtc.syncToSystem();
        lastmin = now.minute;
    }

    gettimeofday(&val, NULL);
    getLocalTime(&t_tm);

    sysinfo.time.hour12 = now.hour % 12 == 0 ? 12 : now.hour % 12;
    sysinfo.time.hour = now.hour;
    sysinfo.time.minute = now.minute;
    sysinfo.time.second = now.second;

    sysinfo.time.valsec = val.tv_sec;
    sysinfo.time.valusec = val.tv_usec;

    sysinfo.date.day = now.day;
    sysinfo.date.month = now.month;
    sysinfo.date.year = now.year;

    String months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    sysinfo.date.dow = rtc.getDayOfWeek(now.day, now.month, now.year);
    sysinfo.date.dayname = days[sysinfo.date.dow];
    sysinfo.date.monthname = months[now.month - 1];

    char time[11];
    strftime(time, sizeof(time), "%a %b %e", &t_tm);
    sysinfo.date.strdate = time;
    strftime(time, sizeof(time), "%D", &t_tm);
    sysinfo.date.numdate = time;
    sysinfo.date.doy = t_tm.tm_yday;
}