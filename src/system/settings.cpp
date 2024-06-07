#include "ArduinoLog.h"
#include "powermgm.hpp"
#include "system.hpp"
#include "settings.hpp"
#include "Preferences.h"

Preferences settings;

bool settingsPeriodic(EventBits_t event, void *arg)
{
    return true;
}

String getstringarray;
const char *getString(const char *key, const char *def, Preferences *pref = &settings)
{
    if (!pref->isKey(key)) {
        pref->putBytes(key, def, strlen(def));
        return (char *)def;
    }

    size_t len = pref->getBytesLength(key);
    char str[len + 1];
    pref->getBytes(key, str, len);

    getstringarray = str;
    getstringarray = getstringarray.substring(0, len);

    Log.verboseln("Getting String Key: %s, Value: %s, Length: %d", key, getstringarray.c_str(), len);

    if (len == 0)
        return def;
    else
        return getstringarray.c_str();
}

bool settingsInit(EventBits_t event, void *arg)
{
    settings.begin("settings");

    sysinfo.health.goal = settings.getUInt("stepgoal", 10000);
    sysinfo.ble.name = getString("ble_name", "New G-Watch");

    powermgmRegisterCB(settingsPeriodic, POWERMGM_LOOP_AWAKE, "SettingsPeriodic");
    return true;
}

bool settingssetup = powermgmRegisterCBPrio(settingsInit, POWERMGM_INIT, "SettingsInit", CALL_CB_FIRST);