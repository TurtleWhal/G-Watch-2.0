#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"

#include "Preferences.h"

Preferences preferences;

enum SettingType
{
    SETTING_TYPE_SWITCH,
    SETTING_TYPE_TEXT,
    SETTING_TYPE_NUMBER
};

typedef struct
{
    String title;
    SettingType type;
} Setting_t;

lv_obj_t *settingsscr;
int8_t settingsx = -1, settingsy = 0;

void createSetting(Setting_t *data)
{
    lv_obj_t *setting = lv_obj_create(settingsscr);
    lv_obj_set_size(setting, 180, 40);
    lv_obj_set_style_radius(setting, 20, LV_PART_MAIN);

    lv_obj_t *title = lv_label_create(setting);
    lv_label_set_text(title, data->title.c_str());

    switch (data->type)
    {
    case SETTING_TYPE_SWITCH:
    {
        lv_obj_t *switchobj = lv_switch_create(setting);
        break;
    }
    case SETTING_TYPE_TEXT:
    {
        lv_obj_t *text = lv_textarea_create(setting);
        break;
    }
    case SETTING_TYPE_NUMBER:
    {
        lv_obj_t *number = lv_textarea_create(setting);
        break;
    }
    }
}

bool settingsperiodic(EventBits_t event, void *arg)
{
    // if (ON_CURRENT_SCREEN(notifx, notify))
    // {
    //     Log.verboseln("on Notif screen");
    // }

    return true;
}

bool settingsscreate(EventBits_t event, void *arg)
{
    settingsscr = screenCreate(settingsx, settingsy);

    lv_obj_set_flex_flow(settingsscr, LV_FLEX_FLOW_COLUMN);

    Setting_t setting1;
    setting1.title = "Switch";
    setting1.type = SETTING_TYPE_SWITCH;

    Setting_t setting2;
    setting2.title = "Text";
    setting2.type = SETTING_TYPE_TEXT;

    Setting_t setting3;
    setting3.title = "Number";
    setting3.type = SETTING_TYPE_NUMBER;

    createSetting(&setting1);
    createSetting(&setting2);
    createSetting(&setting3);

    preferences.begin("Settings", false);

    powermgmRegisterCB(settingsperiodic, POWERMGM_LOOP_AWAKE, "settingsscreeninit");
    return true;
}

bool settingssetup = powermgmRegisterCBPrio(settingsscreate, POWERMGM_INIT, "settingsscreeninit", CALL_CB_LAST);