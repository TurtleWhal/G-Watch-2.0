#include "powermgm.hpp"
#include "motor.hpp"
#include "Arduino.h"

bool vibrating;
uint32_t count;
uint32_t interval_ms;

bool motorPeriodic(EventBits_t event, void *arg)
{
    static uint32_t lastms;

    if (millis() - lastms > interval_ms && vibrating)
    {
        // digitalWrite(MOTOR_PIN1, (count % 2) ? LOW : HIGH );
        digitalWrite(MOTOR_PIN1, count % 2 );
#ifdef WAVESHARE_ESP32_LCD
        // digitalWrite(MOTOR_PIN2, (count % 2) ? LOW : HIGH );
        digitalWrite(MOTOR_PIN2, count % 2);
#endif

        if (!--count)
        {
            vibrating = false;
            digitalWrite(MOTOR_PIN1, LOW);
#ifdef WAVESHARE_ESP32_LCD
            digitalWrite(MOTOR_PIN2, LOW);
#endif
            count = 0;
            interval_ms = 0xFFFFFFFF;
        }
        lastms = millis();
    }
    return true;
}

bool motorInit(EventBits_t event, void *arg)
{

    pinMode(MOTOR_PIN1, OUTPUT);
#ifdef WAVESHARE_ESP32_LCD
    pinMode(MOTOR_PIN2, OUTPUT);
#endif

    powermgmRegisterCB(motorPeriodic, POWERMGM_LOOP, "MotorPeriodic");

    // motorVibrate(1, 500);

    return true;
}

void motorVibrate(uint32_t cnt, uint32_t interval)
{
    count = cnt * 2;
    interval_ms = interval;
    vibrating = true;
}

bool motorsetup = powermgmRegisterCBPrio(motorInit, POWERMGM_INIT, "MotorInit", CALL_CB_FIRST);