#include "Arduino.h"
#include "ArduinoLog.h"
#include "esp_pm.h"

#include "CST816S.h"
#include "lvgl.h"

#include "powermgm.hpp"
#include "display.hpp"
#include "motor.hpp"
#include "system.hpp"

extern CST816S touch;

#ifdef LILYGO_TWATCH_2021
esp_pm_config_esp32_t pm_config;
#else
esp_pm_config_esp32s3_t pm_config;
#endif

esp_pm_lock_handle_t lvgl_lock;

static callback_t *powermgmCallback = NULL;

#define SLEEP_TIMER_MS 30000

#define TIRED_TIMER_RATIO 0.4
#define TIRED_BL_RATIO 0.4

long sleepTimer = 0;
uint8_t prevBacklight = 100;
bool sleeping = false;
bool tired = false;

void setSpeed(int mhz)
{
    pm_config.max_freq_mhz = mhz;
    pm_config.min_freq_mhz = 80;
    pm_config.light_sleep_enable = true;
    esp_pm_configure(&pm_config);
}

void powermgmSleep()
{
    setBacklightGradual(0, 1000);

    powermgmSendEvent(POWERMGM_SLEEP);

    sleeping = true;
    tired = false;
    sysinfo.sleeping = true;

    setSpeed(80);
}

void powermgmWakeup()
{
    if (sleeping || tired)
        setSpeed(240);

    sleepTimer = millis();

    if (sleeping)
    {
        sleeping = false;
        sysinfo.sleeping = false;
        powermgmSendEvent(POWERMGM_WAKEUP);
        setBacklightGradual(prevBacklight, 150);
    }

    if (tired)
    {
        tired = false;
        setBacklightGradual(prevBacklight, 150);
    }
}

void powermgmTickle()
{
    powermgmWakeup();
}

void powermgmLoop()
{
    if (!sleeping)
    {
        if (!tired)
        {
            if (millis() - sleepTimer > SLEEP_TIMER_MS * TIRED_TIMER_RATIO)
            {
                setSpeed(80);
                tired = true;
                prevBacklight = getBacklight();
                setBacklightGradual(prevBacklight * TIRED_BL_RATIO, 500);
            }
        }
        else if (sleepTimer + SLEEP_TIMER_MS < millis())
        {
            powermgmSleep();
        }

        callback_send(powermgmCallback, POWERMGM_LOOP_AWAKE, nullptr);
    }
    else
    {
        delay(50);

        if (touch.available())
            powermgmTickle();
    }

    callback_send(powermgmCallback, POWERMGM_LOOP, nullptr);
}

void powermgmInit()
{
    // callback_init("powermgm");

    // powermgmRegisterCB(exampleFunc, POWERMGM_LOOP, "ExampleFunc");

    callback_send(powermgmCallback, POWERMGM_INIT, nullptr);

    // gpio_install_isr_service(0);
    // gpio_isr_handler_add((gpio_num_t)TP_INT, powermgmTickle, NULL);
    // gpio_isr_handler_add((gpio_num_t)0, powermgmTickle, NULL);
    // attachInterrupt(TP_INT, powermgmTickle, RISING);
    // attachInterrupt(0, powermgmTickle, RISING);

    if (!sysinfo.donotdisturb)
        motorVibrate(3, 100);

    setBacklightGradual(100, 1000);
    powermgmTickle();

    pm_config.max_freq_mhz = 240;
    pm_config.min_freq_mhz = 80;
    pm_config.light_sleep_enable = true;
    esp_pm_configure(&pm_config);
    // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    // esp_log_level_set("sleep", ESP_LOG_WARN);

    // esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, NULL, &lvgl_lock);
}

bool powermgmRegisterCB(CALLBACK_FUNC cb_func, EventBits_t event, const char *id)
{
    if (powermgmCallback == NULL)
    {
        powermgmCallback = callback_init("powermgm");
        // ASSERT(powermgmCallback, "powermgm callback alloc failed");
    }
    // Log.verboseln("Registering powermgm callback: %s", id);
    bool ret = callback_register(powermgmCallback, event, cb_func, id);

    if (!ret)
        // Log.verboseln("Successfully registered callback: %s", id);
        // else
        Log.verboseln("Failed to register callback: %s", id);

    return (ret);
}

bool powermgmRegisterCBPrio(CALLBACK_FUNC cb_func, EventBits_t event, const char *id, callback_prio_t prio)
{
    if (powermgmCallback == NULL)
    {
        powermgmCallback = callback_init("powermgm");
        // ASSERT(powermgmCallback, "powermgm callback alloc failed");
    }
    // Log.verboseln("Registering powermgm callback: %s with prio: %d", id, prio);
    bool ret = callback_register_with_prio(powermgmCallback, event, cb_func, id, prio);

    if (!ret)
        // Log.verboseln("Successfully registered callback: %s", id);
        // else
        Log.verboseln("Failed to register callback: %s with priority: %d", id, prio);

    return (ret);
}

bool powermgmSendEvent(EventBits_t event)
{
    return (callback_send(powermgmCallback, event, nullptr));
}