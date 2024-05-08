#include "Arduino.h"
#include "ArduinoLog.h"
#include "esp_pm.h"

#include "powermgm.hpp"
#include "display.hpp"

#ifdef LILYGO_TWATCH_2021
esp_pm_config_esp32_t pm_config;
#else
esp_pm_config_esp32s3_t pm_config;
#endif
esp_pm_lock_handle_t lvgl_lock;

static callback_t *powermgmCallback = NULL;

void powermgmLoop()
{
    callback_send(powermgmCallback, POWERMGM_LOOP, nullptr);
}

void powermgmInit()
{
    // pm_config.max_freq_mhz = 240;
    // pm_config.min_freq_mhz = 80;
    // pm_config.light_sleep_enable = false;
    // ESP_ERROR_CHECK(esp_pm_configure(&pm_config));

    // callback_init("powermgm");

    // powermgmRegisterCB(exampleFunc, POWERMGM_LOOP, "ExampleFunc");

    callback_send(powermgmCallback, POWERMGM_INIT, nullptr);
    
    setBacklightGradual(100, 1000);
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