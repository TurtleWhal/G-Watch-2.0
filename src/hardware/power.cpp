#include "power.hpp"
#include "system.hpp"
#include "powermgm.hpp"
#include "ble.hpp"
#include <ArduinoLog.h>
#include "movingAvg.h"

#define PWR_ON 5
#define CHRG_PIN 2

#ifdef LILYGO_TWATCH_2021
#define CHARGING (!digitalRead(CHRG_PIN) || voltage > 4000)
#define VOLT_MULT 2
#else
#define CHARGING (voltage > 4000)
#define VOLT_MULT 3
#endif

float percentage = 0;
float voltage = 0.0;
bool charging = false;

// movingAvg voltFilter(100);
// movingAvg percentFilter(100);
movingAvg voltFilter(1);
movingAvg percentFilter(1);

static const float Qcm[2][11] =
    {
        /* 280mah battery */ //{2671, 3079, 3148, 3211, 3282, 3335, 3379, 3441, 3522, 3611, 3740}, //discharge

        /* 600mah battery */
        {2671, 3166, 3246, 3335, 3441, 3497, 3580, 3593, 3662, 3688, 3740}, // discharge
        {4017, 4020, 4036, 4039, 4043, 4043, 4060, 4073, 4083, 4097, 4200}  // charge
};

bool sendPowerBLE(EventBits_t event, void *arg)
{
    sendBLEf("{t:\"status\", bat:%i, chg:%i, volt:%f}", sysinfo.bat.percent, CHARGING ? 1 : 0, voltage / 1000);
    return true;
}

void updatePower()
{

    // Log.verboseln("voltage: %f, voltFiltered: %f", v, voltFilter.reading(v * 1000) / 1000);
    // Serial.print("voltage: ");
    // Serial.print(v);
    // Serial.print(", voltFiltered: ");
    // Serial.println(voltage);

    if (sysinfo.bat.voltage != voltage)
    {
        sysinfo.bat.voltage = voltage;

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

            // if (sysinfo.bat.percent != percentFilter.reading((int)percentage + 0.5))
            if (sysinfo.bat.percent != (int)percentage + 0.5)
            {
                // sysinfo.bat.percent = percentFilter.getAvg();
                sysinfo.bat.percent = (int)percentage + 0.5;
                sendPowerBLE((EventBits_t)NULL, NULL);
            }
        }
        else
        {
            percentage = 100.0;
            sysinfo.bat.percent = 100;
        }
    }
}

bool powerPeriodic(EventBits_t event, void *arg)
{
    static uint32_t last = 0;

    float v = ((analogRead(BAT_ADC) * 3300 * VOLT_MULT) / 4096) + 200;
    // voltage = voltFilter.reading(v * 1000) / 1000;
    voltage = v;

    if (charging != CHARGING)
    {
        if (CHARGING)
        {
            powermgmSendEvent(POWERMGM_PLUGGED_IN);
        }
        else
        {
            powermgmSendEvent(POWERMGM_UNPLUGGED);
        }

        charging = CHARGING;
        sysinfo.bat.charging = charging;
        
        updatePower();
    }

    if (last + 10000 < millis())
    {
        updatePower();
        last = millis();
    }

    return true;
}

bool powerInit(EventBits_t event, void *arg)
{
    pinMode(BAT_ADC, INPUT);

#ifdef LILYGO_TWATCH_2021
    pinMode(PWR_ON, OUTPUT);
    pinMode(CHRG_PIN, INPUT_PULLUP);
    digitalWrite(PWR_ON, HIGH);
#endif // LILYGO_TWATCH_2021

    voltFilter.begin();
    percentFilter.begin();

    powermgmRegisterCB(sendPowerBLE, POWERMGM_BLE_CONNECT, "PowerConnectBLE");

    powermgmRegisterCB(powerPeriodic, POWERMGM_LOOP, "PowerPeriodic");
    return true;
}

bool powersetup = powermgmRegisterCBPrio(powerInit, POWERMGM_INIT, "PowerInit", CALL_CB_LAST);