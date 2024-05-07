#include "Arduino.h"
#include "esp_pm.h"

#include "powermgm.hpp"

#ifdef LILYGO_TWATCH_2021
esp_pm_config_esp32_t pm_config;
#else
esp_pm_config_esp32s3_t pm_config;
#endif
esp_pm_lock_handle_t lvgl_lock;

static callback_t *powermgmCallback = NULL;

static bool exampleFunc(EventBits_t event, void *arg) {
    Serial.println("ExampleFunc");
}

// bool temp = powermgmRegisterCB(exampleFunc, POWERMGM_INIT, "ExampleFunc");

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

    powermgmRegisterCB(exampleFunc, POWERMGM_LOOP, "ExampleFunc");

    callback_send(powermgmCallback, POWERMGM_INIT, nullptr);
}

bool powermgmRegisterCB(CALLBACK_FUNC cb_func, EventBits_t event, const char *id)
{
    if (powermgmCallback == NULL)
    {
        powermgmCallback = callback_init("powermgm");
        // ASSERT(powermgmCallback, "powermgm callback alloc failed");
    }
    return (callback_register(powermgmCallback, event, cb_func, id));
}