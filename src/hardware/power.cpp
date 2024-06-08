#include "power.hpp"
#include "system.hpp"
#include "powermgm.hpp"
#include "ble.hpp"
#include <ArduinoLog.h>
#include "movingAvg.h"
#include "preferences.h"

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

Preferences storage;

float Qcm[2][11] =
    {
        /* 280mah battery */ //{2671, 3079, 3148, 3211, 3282, 3335, 3379, 3441, 3522, 3611, 3740}, //discharge

        /* 600mah battery */
        {2671, 3166, 3246, 3335, 3441, 3497, 3580, 3593, 3662, 3688, 3740}, // discharge
        {4017, 4020, 4036, 4039, 4043, 4043, 4060, 4073, 4083, 4097, 4200}  // charge
};

bool sendPowerBLE(EventBits_t event, void *arg)
{
    // sendBLEf("{t:\"status\", bat:%i, chg:%i, volt:%f}", sysinfo.bat.percent, CHARGING ? 1 : 0, voltage / 1000);
    return true;
}

void checkPower()
{
    if (voltage < 2800)
    {
        Serial.printf("Sleeping, Voltage: %f\n", voltage);

        if (storage.getBool("discharging"))
        {
            int points = 0;
            while (storage.isKey(("d_data_" + String(points)).c_str()))
                points++;

            for (int i = 0; i < 10; i++)
                storage.putFloat(("qcmd" + String(i)).c_str(), storage.getFloat(("d_data_" + String((points / 10) * i)).c_str()));
        }

        esp_deep_sleep(5 * 60 * 1000 * 1000); // 5 minutes
    }
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

            storage.putBool("discharging", false);

            if (voltage < 3000)
                storage.putBool("charging", true);
        }
        else
        {
            powermgmSendEvent(POWERMGM_UNPLUGGED);

            storage.putBool("charging", false);

            if (voltage > 4100)
                storage.putBool("discharging", true);
        }

        storage.putUInt("d_points", 0);
        storage.putUInt("c_points", 0);

        int i = 0;
        while (storage.isKey(("d_data_" + String(i)).c_str()))
        {
            storage.remove(("d_data_" + String(i)).c_str());
            i++;
        }

        i = 0;
        while (storage.isKey(("c_data_" + String(i)).c_str()))
        {
            storage.remove(("c_data_" + String(i)).c_str());
            i++;
        }

        charging = CHARGING;
        sysinfo.bat.charging = charging;

        updatePower();
    }

    if (last + 10000 < millis())
    {
        updatePower();
        last = millis();

        checkPower();

        if (storage.getBool("charging"))
        {
            int points = 0;
            while (storage.isKey(("c_data_" + String(points)).c_str()))
                points++;

            for (int i = 0; i < 10; i++)
                storage.putFloat(("qcmc" + String(i)).c_str(), storage.getFloat(("c_data_" + String((points / 10) * i)).c_str()));
        }

        static int lastmin = -1;

        if (lastmin > (sysinfo.time.minute + 10) % 60 && voltage > 2800)
        {
            if (storage.getBool("discharging"))
            {
                String name = "d_data_" + String(storage.getUInt("d_points"));
                storage.putFloat(name.c_str(), voltage);

                storage.putUInt("d_points", storage.getUInt("d_points") + 1);
            }

            if (storage.getBool("charging"))
            {
                String name = "c_data_" + String(storage.getUInt("c_points"));
                storage.putFloat(name.c_str(), voltage);

                storage.putUInt("c_points", storage.getUInt("c_points") + 1);
            }
        }
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

    storage.begin("battery");

    if (storage.isKey("qcmd0"))
    {
        // Qcm[0] = {storage.getFloat("qcmd0"), storage.getFloat("qcmd1"), storage.getFloat("qcmd2"), storage.getFloat("qcmd3"), storage.getFloat("qcmd4"), storage.getFloat("qcmd5"), storage.getFloat("qcmd6"), storage.getFloat("qcmd7"), storage.getFloat("qcmd8"), storage.getFloat("qcmd9")};
        for (int i = 0; i < 10; i++)
            Qcm[0][i] = storage.getFloat(("qcmd" + String(i)).c_str());
    }

    if (storage.isKey("qcmc0"))
    {
        // Qcm[1] = {storage.getFloat("qcmc0"), storage.getFloat("qcmc1"), storage.getFloat("qcmc2"), storage.getFloat("qcmc3"), storage.getFloat("qcmc4"), storage.getFloat("qcmc5"), storage.getFloat("qcmc6"), storage.getFloat("qcmc7"), storage.getFloat("qcmc8"), storage.getFloat("qcmc9")};
        for (int i = 0; i < 10; i++)
            Qcm[1][i] = storage.getFloat(("qcmc" + String(i)).c_str());
    }

    Serial.printf("Charging  Qcm: %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f\n", Qcm[0][0], Qcm[0][1], Qcm[0][2], Qcm[0][3], Qcm[0][4], Qcm[0][5], Qcm[0][6], Qcm[0][7], Qcm[0][8], Qcm[0][9]);
    Serial.printf("Discharge Qcm: %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f, %.0f\n", Qcm[1][0], Qcm[1][1], Qcm[1][2], Qcm[1][3], Qcm[1][4], Qcm[1][5], Qcm[1][6], Qcm[1][7], Qcm[1][8], Qcm[1][9]);

    powermgmRegisterCB(sendPowerBLE, POWERMGM_BLE_CONNECT, "PowerConnectBLE");
    powermgmRegisterCB(powerPeriodic, POWERMGM_LOOP, "PowerPeriodic");
    return true;
}

bool powersetup = powermgmRegisterCBPrio(powerInit, POWERMGM_INIT, "PowerInit", CALL_CB_LAST);