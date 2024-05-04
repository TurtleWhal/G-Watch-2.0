#pragma GCC diagnostic ignored "-Wnarrowing"

#include "Arduino.h"
#include "lvgl.h"
#include "system.hpp"
#include "screens.hpp"

class ClockScreen
{
private:
    lv_obj_t *hourhand;
    lv_obj_t *minutehand;
    lv_obj_t *secondhand;
    lv_point_precise_t hourpoints[2] = {{120, 120}, {120, 20}};
    lv_point_precise_t minutepoints[2] = {{120, 120}, {120, 20}};
    lv_point_precise_t secondpoints[2] = {{120, 120}, {120, 20}};

public:
    void create(lv_obj_t *screen, uint8_t x, uint8_t y)
    {
        scr = screenCreate(x, y);

        static lv_style_t style_hour;
        static lv_style_t style_minute;
        static lv_style_t style_second;
        lv_style_init(&style_hour);
        lv_style_init(&style_minute);
        lv_style_init(&style_second);

        lv_style_set_line_width(&style_hour, 15);
        lv_style_set_line_width(&style_minute, 10);
        lv_style_set_line_width(&style_second, 5);

        lv_style_set_line_color(&style_hour, lv_theme_get_color_primary(scr));
        lv_style_set_line_color(&style_minute, lv_color_lighten(lv_theme_get_color_secondary(scr), 0));
        lv_style_set_line_color(&style_second, lv_color_lighten(lv_theme_get_color_secondary(scr), 150));

        lv_style_set_line_rounded(&style_hour, true);
        lv_style_set_line_rounded(&style_minute, true);
        lv_style_set_line_rounded(&style_second, true);

        // lv_obj_t *linebox = lv_obj_create(scr);
        // lv_obj_set_size(linebox, 240, 240);
        // lv_obj_set_align(linebox, LV_ALIGN_CENTER);

        hourhand = lv_line_create(scr);
        minutehand = lv_line_create(scr);
        secondhand = lv_line_create(scr);

        lv_line_set_points(hourhand, hourpoints, 2);
        lv_line_set_points(minutehand, minutepoints, 2);
        lv_line_set_points(secondhand, secondpoints, 2);

        lv_obj_add_style(hourhand, &style_hour, 0);
        lv_obj_add_style(minutehand, &style_minute, 0);
        lv_obj_add_style(secondhand, &style_second, 0);

        // for some reason lines are offset by 15px so this fixes it
        lv_obj_align(secondhand, LV_ALIGN_TOP_LEFT, -15, -15);
        lv_obj_align(minutehand, LV_ALIGN_TOP_LEFT, -15, -15);
        lv_obj_align(hourhand, LV_ALIGN_TOP_LEFT, -15, -15);

        // lv_obj_t *center = lv_obj_create(scr);
        // lv_obj_set_size(center, 5, 5);
        // lv_obj_align(center, LV_ALIGN_CENTER, 0, 0);
    };

    void periodic()
    {
        int secondlength = 100;
        // double secondangle = fmod(-((millis() % 60000) * 0.006), 360);
        double secondangle = 180 - (sysinfo.time.second * 6 + sysinfo.time.millis * 0.006);
        secondpoints[1] = (lv_point_precise_t){120 + (sin(DEG_TO_RAD * secondangle) * secondlength), 120 + (cos(DEG_TO_RAD * secondangle) * secondlength)};

        int minlength = 90;
        // int minuteangle = fmod(-((millis() % 3600000) * 0.0001), 360);
        double minuteangle = 180 - (sysinfo.time.minute * 6 + sysinfo.time.second * 0.1);
        minutepoints[1] = (lv_point_precise_t){120 + (sin(DEG_TO_RAD * minuteangle) * minlength), 120 + (cos(DEG_TO_RAD * minuteangle) * minlength)};

        int hourlength = 70;
        // int hourangle = fmod(-((millis() % 216000000) * 0.000001666666666666666), 360);
        double hourangle = 180 - (sysinfo.time.hour12 * 30 + sysinfo.time.minute * 0.1);
        hourpoints[1] = (lv_point_precise_t){120 + (sin(DEG_TO_RAD * hourangle) * hourlength), 120 + (cos(DEG_TO_RAD * hourangle) * hourlength)};

        // minutepoints[0] = {0, 120};
        // minutepoints[1] = {120, 0};

        lv_line_set_points(hourhand, hourpoints, 2);
        lv_line_set_points(minutehand, minutepoints, 2);
        lv_line_set_points(secondhand, secondpoints, 2);
    };
    lv_obj_t *scr;
};