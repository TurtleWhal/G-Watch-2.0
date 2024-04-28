#include "power.hpp"
#include "system.hpp"
#include <ArduinoLog.h>

#define BAT_PIN 36
#define PWR_ON 5
#define CHRG_PIN 2

float percentage = 0;
float voltage = 0.0;
bool charging = false;

static const float Qcm[2][11] =
    {
        /* 280mah battery */ //{2671, 3079, 3148, 3211, 3282, 3335, 3379, 3441, 3522, 3611, 3740}, //discharge

        /* 600mah battery */
        {2671, 3166, 3246, 3335, 3441, 3497, 3580, 3593, 3662, 3688, 3740}, // discharge
        {4017, 4020, 4036, 4039, 4043, 4043, 4060, 4073, 4083, 4097, 4200}  // charge
};

void powerInit()
{
    pinMode(PWR_ON, OUTPUT);
    pinMode(BAT_PIN, INPUT);
    pinMode(CHRG_PIN, INPUT_PULLUP);
    digitalWrite(PWR_ON, HIGH);
}

void powerPeriodic()
{
    voltage = ((analogRead(BAT_PIN) * 3300 * 2) / 4096) + 200;
    sysinfo.bat.voltage = voltage;

    charging = (!digitalRead(CHRG_PIN) || voltage > 4000);

    uint8_t chrgint = charging ? 1 : 0;

    int8_t i = 10;
    while (voltage <= Qcm[chrgint][i])
    {
        if (i == 0)
            break;
        else
            i--;
    }

    if (i < 10)
    {
        float vol_section = Qcm[chrgint][i + 1] - Qcm[chrgint][i];
        float decade = i * 10.0;
        percentage = constrain(decade + 10.0 * ((voltage - Qcm[chrgint][i]) / vol_section), 0.0, 100.0);
        sysinfo.bat.percent = (int)percentage + 0.5;
    }
    else
    {
        percentage = 100.0;
        sysinfo.bat.percent = 100;
    }
}