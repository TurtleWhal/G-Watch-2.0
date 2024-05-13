#include "powermgm.hpp"
#include "motor.hpp"
#include "Arduino.h"

int64_t timer;
bool vibrating;

bool motorPeriodic(EventBits_t event, void *arg)
{
    static uint32_t lastms;
    if (vibrating)
    {
        timer -= millis() - lastms;
        lastms = millis();

        if (timer < 0)
        {
            vibrating = false;

            analogWrite(MOTOR_PIN1, 0);
            analogWrite(MOTOR_PIN2, 0);
        }
    }
    return true;
}

bool motorInit(EventBits_t event, void *arg)
{

    pinMode(MOTOR_PIN1, OUTPUT);
    pinMode(MOTOR_PIN2, OUTPUT);

    powermgmRegisterCB(motorPeriodic, POWERMGM_LOOP, "MotorPeriodic");

    motorVibrate(50, 500);

    return true;
}

// timevalue is unix time in seconds, timezone is offset in hours
void motorVibrate(uint8_t intensity, uint32_t ms)
{
    analogWrite(MOTOR_PIN1, intensity * 2.55);
    analogWrite(MOTOR_PIN2, intensity * 2.55);

    vibrating = true;
    timer = ms;
}

bool motorsetup = powermgmRegisterCBPrio(motorInit, POWERMGM_INIT, "MotorInit", CALL_CB_FIRST);