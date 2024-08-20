#include "Arduino.h"
#include "ArduinoLog.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"
#include "powermgm.hpp"
#include "settings.hpp"
#include "display.hpp"
#include "fonts/fonts.hpp"

#include "Preferences.h"

enum SettingType
{
    SETTING_TYPE_SWITCH,
    SETTING_TYPE_TEXT,
    SETTING_TYPE_NUMBER,
    SETTING_TYPE_BUTTON,
    SETTING_TYPE_ICON_BUTTON,
    SETTING_TYPE_ICON_BUTTON_GROUP
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

lv_obj_t *bright;
lv_obj_t *uptime;

lv_obj_t *createSetting(Setting_t *data)
{
    lv_obj_t *setting = lv_obj_create(settingsscr);
    lv_obj_set_size(setting, 180, 40);
    lv_obj_set_style_radius(setting, 20, LV_PART_MAIN);
    lv_obj_set_scroll_dir(setting, LV_DIR_NONE);

    lv_obj_t *title = lv_label_create(setting);
    lv_label_set_text(title, data->title.c_str());

    lv_obj_align(title, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_flag(setting, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t *input;

    switch (data->type)
    {
    case SETTING_TYPE_SWITCH:
    {
        lv_obj_t *switchobj = lv_switch_create(setting);
        lv_obj_align(switchobj, LV_ALIGN_RIGHT_MID, 12, 0);

        lv_obj_add_event_cb(switchobj, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(switchobj);

        input = switchobj;

        break;
    }
    case SETTING_TYPE_TEXT:
    {
        lv_obj_t *text = lv_textarea_create(setting);
        lv_obj_set_size(text, 100, 30);
        lv_obj_align(text, LV_ALIGN_RIGHT_MID, 12, 0);
        lv_obj_set_style_radius(text, 15, LV_PART_MAIN);
        lv_obj_set_style_bg_color(text, lv_color_black(), LV_PART_MAIN);
        lv_textarea_set_one_line(text, true);

        lv_obj_add_event_cb(text, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(text);

        input = text;

        attachKeyboard(text, KEYBOARD_CHARS_ALL);
        break;
    }
    case SETTING_TYPE_NUMBER:
    {
        lv_obj_t *number = lv_textarea_create(setting);
        lv_obj_set_size(number, 80, 30);
        lv_obj_align(number, LV_ALIGN_RIGHT_MID, 12, 0);
        lv_obj_set_style_radius(number, 15, LV_PART_MAIN);
        lv_obj_set_scroll_dir(number, LV_DIR_HOR);
        lv_obj_set_style_bg_color(number, lv_color_black(), LV_PART_MAIN);
        lv_textarea_set_one_line(number, true);
        lv_obj_set_style_text_align(number, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN);

        lv_obj_add_event_cb(number, data->onchange, LV_EVENT_VALUE_CHANGED, NULL);
        data->init(number);

        input = number;

        attachKeyboard(number, KEYBOARD_CHARS_NUMBERS);
        break;
    }
    case SETTING_TYPE_BUTTON:
    {
        lv_obj_set_style_border_opa(setting, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_bg_color(setting, lv_theme_get_color_primary(setting), LV_PART_MAIN);
        lv_obj_add_event_cb(setting, data->onchange, LV_EVENT_CLICKED, NULL);

        lv_obj_center(title);
        return setting;
    }
    case SETTING_TYPE_ICON_BUTTON:
    {
        lv_obj_set_size(setting, 50, 50);
        lv_obj_set_style_radius(setting, 25, LV_PART_MAIN);

        lv_obj_set_style_border_opa(setting, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_bg_color(setting, lv_theme_get_color_primary(setting), LV_PART_MAIN);
        lv_obj_set_style_bg_color(setting, lv_color_make(255, 80, 80), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_add_flag(setting, LV_OBJ_FLAG_CHECKABLE);
        lv_obj_add_event_cb(setting, data->onchange, LV_EVENT_CLICKED, NULL);

        lv_obj_center(title);
        SET_SYMBOL_32(title, data->title.c_str());

        data->init(setting);

        return setting;
    }
    case SETTING_TYPE_ICON_BUTTON_GROUP:
    {
        lv_obj_set_height(setting, 50);
        lv_obj_set_style_border_opa(setting, LV_OPA_TRANSP, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(setting, LV_OPA_TRANSP, LV_PART_MAIN);

        lv_obj_set_flex_flow(setting, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(setting, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_BETWEEN);

        lv_obj_set_style_pad_all(setting, 0, LV_PART_MAIN);
        lv_obj_delete(title);

        data->init(setting);

        return setting;
    }
    default:
        return setting;
        break;
    }

    if (lv_obj_get_style_width(setting, LV_PART_MAIN) - lv_obj_get_content_width(title) - 8 < lv_obj_get_style_width(input, LV_PART_MAIN))
    {
        lv_obj_set_height(setting, 80);
        lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);
        lv_obj_align(input, LV_ALIGN_BOTTOM_RIGHT, 10, 10);

        if (data->type != SETTING_TYPE_SWITCH)
        {
            lv_obj_set_width(input, lv_obj_get_width(setting) - 10);
        }

        lv_obj_set_style_radius(input, lv_obj_get_style_radius(setting, LV_PART_MAIN) - 5, LV_PART_MAIN);
    }
    else
        lv_obj_set_style_radius(input, 30, LV_PART_MAIN);

    lv_obj_set_style_border_opa(setting, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_opa(input, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_add_flag(title, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_add_event_cb(setting, [](lv_event_t *e)
                        {
                            lv_obj_t *i = (lv_obj_t *)e->user_data;

                            if (lv_obj_check_type(i, &lv_switch_class))
                            {
                                lv_obj_has_state(i, LV_STATE_CHECKED) ? lv_obj_remove_state(i, LV_STATE_CHECKED) : lv_obj_add_state(i, LV_STATE_CHECKED);
                                lv_obj_send_event(i, LV_EVENT_VALUE_CHANGED, e->param);
                            }

                            lv_obj_send_event(i, LV_EVENT_CLICKED, e->param); }, LV_EVENT_CLICKED, input);

    return setting;
}

bool settingsLoad(EventBits_t event, void *arg)
{

    if (ON_CURRENT_SCREEN(settingsx, settingsy))
    {
        setScroll(LV_DIR_HOR);
        lv_slider_set_value(bright, getBacklight(), LV_ANIM_OFF);
        lv_obj_send_event(bright, LV_EVENT_VALUE_CHANGED, NULL);
    }

    return true;
}

bool settingsperiodic(EventBits_t event, void *arg)
{
    if (ON_CURRENT_SCREEN(settingsx, settingsy))
    {
        //     Log.verboseln("on Settings screen");
        uint32_t mils = millis();
        lv_label_set_text_fmt(uptime, "Uptime: %d:%02d:%02d", mils / 3600000, (mils / 60000) % 60, (mils / 1000) % 60);
    }

    return true;
}

bool settingsscreate(EventBits_t event, void *arg)
{
    settingsscr = screenCreate(settingsx, settingsy);

    lv_obj_set_flex_flow(settingsscr, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(settingsscr, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_t *s1 = lv_obj_create(settingsscr);
    lv_obj_set_size(s1, 0, 20);

    lv_obj_set_scroll_dir(settingsscr, LV_DIR_VER);
    createCurvedScrollbar(settingsscr);

    Setting_t group;
    group.type = SETTING_TYPE_ICON_BUTTON_GROUP;
    group.init = [](lv_obj_t *obj)
    {
        Setting_t dnd;
        dnd.title = FA_DO_NOT_DISTURB;
        dnd.type = SETTING_TYPE_ICON_BUTTON;
        dnd.init = [](lv_obj_t *obj)
        {
            lv_obj_set_state(obj, LV_STATE_CHECKED, settings.getBool("dnd", false));
        };
        dnd.onchange = [](lv_event_t *e)
        {
            settings.putBool("dnd", lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED));
            sysinfo.donotdisturb = settings.getBool("dnd", false);
        };

        Setting_t flash;
        flash.title = FA_FLASHLIGHT;
        flash.type = SETTING_TYPE_ICON_BUTTON;
        flash.onchange = [](lv_event_t *e)
        {
            lv_obj_set_state(lv_event_get_target_obj(e), LV_STATE_CHECKED, false);
            lv_obj_t *scr = lv_obj_create(nullptr);
            lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
            lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

            const long pb = getBacklight();

            lv_obj_add_event_cb(scr, [](lv_event_t *e)
                                {
                                    Serial.println((long)e->user_data);
                                    setBacklight((long)e->user_data);
                                    setScreen(nullptr, LV_SCR_LOAD_ANIM_NONE, 0, true); }, LV_EVENT_CLICKED, (void *)pb);

            setScreen(scr, LV_SCR_LOAD_ANIM_NONE, 0, false);
            setBacklight(100);
        };

        Setting_t ble;
        ble.title = FA_BLUETOOTH;
        ble.type = SETTING_TYPE_ICON_BUTTON;
        ble.onchange = [](lv_event_t *e) {};

        lv_obj_set_parent(createSetting(&flash), obj);
        lv_obj_set_parent(createSetting(&dnd), obj);
        lv_obj_set_parent(createSetting(&ble), obj);
    };

    Setting_t blename;
    blename.title = "BLE Name";
    blename.type = SETTING_TYPE_TEXT;
    blename.init = [](lv_obj_t *obj)
    {
        lv_textarea_set_text(obj, getString("ble_name", "G-Watch", &settings));
    };
    blename.onchange = [](lv_event_t *e)
    {
        const char *text = lv_textarea_get_text(lv_event_get_target_obj(e));
        settings.putBytes("ble_name", text, strlen(text));
    };

    // Setting_t disturb;
    // disturb.title = "Do Not Disturb";
    // disturb.type = SETTING_TYPE_SWITCH;
    // disturb.init = [](lv_obj_t *obj)
    // {
    //     lv_obj_set_state(obj, LV_STATE_CHECKED, settings.getBool("dnd", false));
    // };
    // disturb.onchange = [](lv_event_t *e)
    // {
    //     settings.putBool("dnd", lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED));
    //     sysinfo.donotdisturb = settings.getBool("dnd", false);
    // };

    Setting_t stepgoal;
    stepgoal.title = "Step Goal";
    stepgoal.type = SETTING_TYPE_NUMBER;
    stepgoal.init = [](lv_obj_t *obj)
    {
        lv_textarea_set_text(obj, String(sysinfo.health.goal).c_str());
    };
    stepgoal.onchange = [](lv_event_t *e)
    {
        const char *text = lv_textarea_get_text(lv_event_get_target_obj(e));
        sysinfo.health.goal = atoi(text);
        settings.putUInt("stepgoal", sysinfo.health.goal);
    };

    Setting_t flipscr;
    flipscr.title = "Flip Screen";
    flipscr.type = SETTING_TYPE_SWITCH;
    flipscr.init = [](lv_obj_t *obj)
    {
        lv_obj_set_state(obj, LV_STATE_CHECKED, settings.getBool("flip", false));

#ifdef LILYGO_TWATCH_2021
        setRotation(lv_obj_has_state(obj, LV_STATE_CHECKED) ? LV_DISPLAY_ROTATION_180 : LV_DISPLAY_ROTATION_0);
#else
        setRotation(lv_obj_has_state(obj, LV_STATE_CHECKED) ? LV_DISPLAY_ROTATION_270 : LV_DISPLAY_ROTATION_90);
#endif
    };
    flipscr.onchange = [](lv_event_t *e)
    {
        settings.putBool("flip", lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED));

#ifdef LILYGO_TWATCH_2021
        setRotation(lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED) ? LV_DISPLAY_ROTATION_180 : LV_DISPLAY_ROTATION_0);
#else
        setRotation(lv_obj_has_state(lv_event_get_target_obj(e), LV_STATE_CHECKED) ? LV_DISPLAY_ROTATION_270 : LV_DISPLAY_ROTATION_90);
#endif
    };

    // Setting_t flash;
    // flash.title = "Flashlight";
    // flash.type = SETTING_TYPE_SWITCH;
    // flash.onchange = [](lv_event_t *e)
    // {
    //     lv_obj_t *scr = lv_obj_create(nullptr);
    //     lv_obj_set_style_bg_color(scr, lv_color_white(), LV_PART_MAIN);
    //     lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);

    //     const long pb = getBacklight();

    //     lv_obj_add_event_cb(scr, [](lv_event_t *e)
    //                         {
    //                             Serial.println((long)e->user_data);
    //                             setBacklight((long)e->user_data);
    //                             setScreen(nullptr, LV_SCR_LOAD_ANIM_NONE, 0, true); }, LV_EVENT_CLICKED, (void *)pb);

    //     setScreen(scr, LV_SCR_LOAD_ANIM_NONE, 0, false);
    //     setBacklight(100);
    // };

    Setting_t rst;
    rst.title = "Reboot";
    rst.type = SETTING_TYPE_BUTTON;
    rst.onchange = [](lv_event_t *e)
    { ESP.restart(); };

    bright = lv_slider_create(settingsscr);
    lv_obj_set_size(bright, 180, 50);
    lv_obj_set_style_bg_opa(bright, LV_OPA_TRANSP, LV_PART_KNOB);

    lv_slider_set_range(bright, -44 / 1.4, 100);
    lv_obj_add_flag(bright, LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t *brightlabel = lv_label_create(bright);
    SET_SYMBOL_40(brightlabel, FA_BRIGHTNESS);
    lv_obj_align(brightlabel, LV_ALIGN_LEFT_MID, 4, 0);
    lv_obj_set_style_text_color(brightlabel, lv_color_white(), LV_PART_MAIN);

    lv_obj_add_event_cb(bright, [](lv_event_t *e)
                        {
                            if (lv_slider_get_value(lv_event_get_target_obj(e)) < 1)
                                lv_slider_set_value(lv_event_get_target_obj(e), 1, LV_ANIM_OFF);

                            setBacklight(lv_slider_get_value(lv_event_get_target_obj(e)));
                            lv_obj_set_x((lv_obj_t *)e->user_data, (lv_slider_get_value(lv_event_get_target_obj(e)) * 1.4) - 4);

                            if (lv_slider_get_value(lv_event_get_target_obj(e)) < 50)
                            {
                                SET_SYMBOL_40((lv_obj_t *)lv_event_get_user_data(e), FA_BRIGHTNESS_LOW);
                            }
                            else
                            {
                                SET_SYMBOL_40((lv_obj_t *)lv_event_get_user_data(e), FA_BRIGHTNESS);
                            } }, LV_EVENT_VALUE_CHANGED, brightlabel);

    lv_slider_set_value(bright, 100, LV_ANIM_OFF);
    lv_obj_send_event(bright, LV_EVENT_VALUE_CHANGED, NULL);

    createSetting(&group);
    createSetting(&stepgoal);
    // createSetting(&disturb);
    // createSetting(&flash);
    createSetting(&flipscr);
    createSetting(&blename);
    createSetting(&rst);

    uptime = lv_label_create(settingsscr);
    lv_obj_t *compdate = lv_label_create(settingsscr);
    lv_label_set_text_fmt(compdate, "Compilation Date:\n%s", __DATE__);
    lv_obj_set_style_text_align(compdate, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN);

    lv_obj_t *s2 = lv_obj_create(settingsscr);
    lv_obj_set_size(s2, 0, 20);

    powermgmRegisterCB(settingsperiodic, POWERMGM_LOOP_AWAKE, "settingsscreeninit");
    powermgmRegisterCB(settingsLoad, POWERMGM_SCREEN_CHANGE, "settingsscreenload");
    return true;
}

bool settingsscreensetup = powermgmRegisterCBPrio(settingsscreate, POWERMGM_INIT, "settingsscreeninit", CALL_CB_LAST);