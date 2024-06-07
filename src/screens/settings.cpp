#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "settings.hpp"

#include "Preferences.h"

enum SettingType
{
    SETTING_TYPE_SWITCH,
    SETTING_TYPE_TEXT,
    SETTING_TYPE_NUMBER
};

void nullFuncObj(lv_obj_t *e) {}
void nullFuncEvt(lv_event_t *e) {}

typedef struct
{
    String title;
    SettingType type;
    void (*init)(lv_obj_t *) = nullFuncObj;
    void (*onchange)(lv_event_t *) = nullFuncEvt;
} Setting_t;

lv_obj_t *settingsscr;
int8_t settingsx = -1, settingsy = 0;

void createSetting(Setting_t *data)
{
    lv_obj_t *setting = lv_obj_create(settingsscr);
    lv_obj_set_size(setting, 180, 40);
    lv_obj_set_style_radius(setting, 20, LV_PART_MAIN);
    lv_obj_set_scroll_dir(setting, LV_DIR_NONE);

    lv_obj_t *title = lv_label_create(setting);
    lv_label_set_text(title, data->title.c_str());

    lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);

    switch (data->type)
    {
    case SETTING_TYPE_SWITCH:
    {
        lv_obj_t *switchobj = lv_switch_create(setting);
        lv_obj_align(switchobj, LV_ALIGN_RIGHT_MID, 10, 0);

        lv_obj_add_event_cb(switchobj, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(switchobj);

        break;
    }
    case SETTING_TYPE_TEXT:
    {
        lv_obj_t *text = lv_textarea_create(setting);
        lv_obj_set_size(text, 100, 30);
        lv_obj_align(text, LV_ALIGN_RIGHT_MID, 10, 0);
        lv_obj_set_style_radius(text, 15, LV_PART_MAIN);
        lv_obj_set_scroll_dir(text, LV_DIR_HOR);

        lv_obj_add_event_cb(text, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(text);

        attachKeyboard(text, KEYBOARD_CHARS_ALL);
        break;
    }
    case SETTING_TYPE_NUMBER:
    {
        lv_obj_t *number = lv_textarea_create(setting);
        lv_obj_set_size(number, 100, 30);
        lv_obj_align(number, LV_ALIGN_RIGHT_MID, 10, 0);
        lv_obj_set_style_radius(number, 15, LV_PART_MAIN);
        lv_obj_set_scroll_dir(number, LV_DIR_HOR);

        lv_obj_add_event_cb(number, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(number);

        attachKeyboard(number, KEYBOARD_CHARS_NUMBERS);
        break;
    }
    }
}

bool settingsLoad(EventBits_t event, void *arg)
{

    if (ON_CURRENT_SCREEN(settingsx, settingsy))
    {
        setScroll(LV_DIR_HOR);
    }

    return true;
}

bool settingsperiodic(EventBits_t event, void *arg)
{
    // if (ON_CURRENT_SCREEN(settingsx, settingsy))
    // {
    //     Log.verboseln("on Settings screen");
    // }

    return true;
}

bool settingsscreate(EventBits_t event, void *arg)
{
    settingsscr = screenCreate(settingsx, settingsy);

    lv_obj_set_flex_flow(settingsscr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(settingsscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *s1 = lv_obj_create(settingsscr);
    lv_obj_set_size(s1, 0, 30);

    lv_obj_set_scroll_dir(settingsscr, LV_DIR_VER);
    createCurvedScrollbar(settingsscr);

    // Setting_t setting1;
    // setting1.title = "Switch";
    // setting1.type = SETTING_TYPE_SWITCH;

    // Setting_t setting2;
    // setting2.title = "Text";
    // setting2.type = SETTING_TYPE_TEXT;

    // Setting_t setting3;
    // setting3.title = "Number";
    // setting3.type = SETTING_TYPE_NUMBER;

    Setting_t blename;
    blename.title = "BLE Name";
    blename.type = SETTING_TYPE_TEXT;
    blename.init = [](lv_obj_t *obj)
    {
        Log.verboseln("Init BLE Name: %s", getString("ble_name", "G-Watch", &settings));
        lv_textarea_set_text(obj, getString("ble_name", "G-Watch", &settings));
    };
    blename.onchange = [](lv_event_t *e)
    {
        const char *text = lv_textarea_get_text(lv_event_get_target_obj(e));
        Log.verboseln("set BLE Name: %s, len: %i", text, strlen(text));
        settings.putBytes("ble_name", text, strlen(text));
    };

    Setting_t disturb;
    disturb.title = "Do Not Disturb";
    disturb.type = SETTING_TYPE_SWITCH;
    disturb.init = [](lv_obj_t *obj)
    {
        lv_obj_set_state(obj, LV_STATE_CHECKED, settings.getBool("dnd", false));
    };
    disturb.onchange = [](lv_event_t *e)
    {
        Log.verboseln("Do Not Disturb change: %i", lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED));
    };

    createSetting(&blename);
    createSetting(&disturb);
    // createSetting(&setting3);
    // createSetting(&setting1);
    // createSetting(&setting2);
    // createSetting(&setting1);
    // createSetting(&setting1);
    // createSetting(&setting1);
    // createSetting(&setting1);
    // createSetting(&setting1);

    lv_obj_t *s2 = lv_obj_create(settingsscr);
    lv_obj_set_size(s2, 0, 20);

    powermgmRegisterCB(settingsperiodic, POWERMGM_LOOP_AWAKE, "settingsscreeninit");
    powermgmRegisterCB(settingsLoad, POWERMGM_SCREEN_CHANGE, "settingsscreenload");
    return true;
}

bool settingsscreensetup = powermgmRegisterCBPrio(settingsscreate, POWERMGM_INIT, "settingsscreeninit", CALL_CB_LAST);