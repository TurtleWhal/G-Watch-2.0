#include "Arduino.h"
#include "ArduinoLog.h"
#include "esp_pm.h"

#include "CST816S.h"
#include "lvgl.h"

#include "powermgm.hpp"
#include "display.hpp"
#include "motor.hpp"

extern CST816S touch;

#ifdef LILYGO_TWATCH_2021
esp_pm_config_esp32_t pm_config;
#else
esp_pm_config_esp32s3_t pm_config;
#endif

esp_pm_lock_handle_t lvgl_lock;

static callback_t *powermgmCallback = NULL;

#define SLEEP_TIMER_MS 30000

long sleepTimer = 0;
uint8_t prevBacklight = 100;
bool sleeping = false;

void powermgmSleep()
{
    prevBacklight = getBacklight();
    setBacklightGradual(0, 3000);
    // setBacklight(0);

    powermgmSendEvent(POWERMGM_SLEEP);

    sleeping = true;

    // pm_config.max_freq_mhz = 240;
    // pm_config.min_freq_mhz = 80;
    // pm_config.light_sleep_enable = true;
    // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
}

void powermgmWakeup()
{
    // pm_config.max_freq_mhz = 240;
    // pm_config.min_freq_mhz = 80;
    // pm_config.light_sleep_enable = false;
    // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    sleepTimer = millis();
    sleeping = false;

    powermgmSendEvent(POWERMGM_WAKEUP);

    setBacklightGradual(prevBacklight, 150);
    // setBacklight(prevBacklight);
}

void powermgmTickle()
{
    if (sleeping)
        powermgmWakeup();
    else
    {
        sleepTimer = millis();
    }
}

void powermgmLoop()
{

    if (!sleeping)
    {

        if (sleepTimer + SLEEP_TIMER_MS < millis())
        {
            powermgmSleep();
        }

        // esp_pm_lock_acquire(lvgl_lock);

        callback_send(powermgmCallback, POWERMGM_LOOP, nullptr);
        callback_send(powermgmCallback, POWERMGM_LOOP_AWAKE, nullptr);

        // esp_pm_lock_release(lvgl_lock);
    }
    else
    {
        // esp_pm_lock_acquire(lvgl_lock);

        callback_send(powermgmCallback, POWERMGM_LOOP, nullptr);

        // esp_pm_lock_release(lvgl_lock);
        delay(50);

        if (touch.available())
            powermgmTickle();
    }
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

    // motorVibrate(1, 400);

    setBacklightGradual(100, 1000);
    // setBacklight(100);

    pm_config.max_freq_mhz = 240;
    pm_config.min_freq_mhz = 80;
    pm_config.light_sleep_enable = true;
    // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    esp_pm_configure(&pm_config);

    esp_pm_lock_create(ESP_PM_NO_LIGHT_SLEEP, 0, NULL, &lvgl_lock);
}

bool powermgmRegisterCB(CALLBACK_FUNC cb_func, EventBits_t event, const char *id)
{
    if (powermgmCallback == NULL)
    {
        powermgmCallback = callback_init("powermgm");
        // ASSERT(powermgmCallback, "powermgm callback alloc failed");
    }
    Log.verboseln("Registering powermgm callback: %s", id);
    bool ret = callback_register(powermgmCallback, event, cb_func, id);

    if (ret)
        Log.verboseln("Successfully registered callback: %s", id);
    else
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
    Log.verboseln("Registering powermgm callback: %s with prio: %d", id, prio);
    bool ret = callback_register_with_prio(powermgmCallback, event, cb_func, id, prio);

    if (ret)
        Log.verboseln("Successfully registered callback: %s", id);
    else
        Log.verboseln("Failed to register callback: %s", id);

    return (ret);
}

bool powermgmSendEvent(EventBits_t event)
{
    return (callback_send(powermgmCallback, event, nullptr));
}