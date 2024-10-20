#include <ArduinoLog.h>
#include "TFT_eSPI.h"
#include "CST816S.h"
#include "lvgl.h"
#include "system.hpp"
#include "display.hpp"
#include "fonts/fonts.hpp"

#include "powermgm.hpp"

TFT_eSPI tft = TFT_eSPI();

lv_display_t *display;
lv_indev_t *indev;

int16_t bgval;
TaskHandle_t backlightHandle = NULL;

CST816S touch(IIC_SDA, IIC_SCL, TP_RST, TP_INT); // sda, scl, rst, irq

void my_flush_cb(lv_display_t *disp, const lv_area_t *area, void *px_map)
{
    // Serial.printf("Flush cb area: (%i, %i) (%i, %i) Pixel map: %i\n", area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map);

    uint16_t *buf16 = (uint16_t *)px_map;

    tft.startWrite();
    tft.pushImageDMA(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1), buf16);
    tft.endWrite();

    lv_display_flush_ready(disp);
}

void my_input_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;
    static bool touching = false;
    static bool ignore = false; // ignore touch if waking up screen

    if (touch.available())
    {
        // if (ignore)
        //     return;

        // if (sysinfo.sleeping)
        //     ignore = true;

        if (!touching)
            Log.verboseln("Screen Touched at: %d, %d", touch.data.x, touch.data.y);

        touching = true;

        last_x = touch.data.x;
        last_y = touch.data.y;
        data->state = LV_INDEV_STATE_PR;

        powermgmTickle();
    }
    else
    {
        touching = false;
        ignore = false;
        data->state = LV_INDEV_STATE_REL;
    }
    data->point.x = last_x;
    data->point.y = last_y;
}

// void lvglHandle(void *params)
// {
//     for (;;)
//     {
//         Serial.println(millis());
//         lv_task_handler();
//     }
// }

static uint32_t lvTick()
{
    return millis();
}

uint16_t oldBacklight = 0;
uint32_t endtime = 0;
uint32_t starttime = 0;
float k = 0;
bool adjust = false;

void setBacklightGradual(int16_t val, uint32_t ms)
{
    int32_t tempbk;
    oldBacklight = getBacklight();
    tempbk = (val - oldBacklight);
    endtime = millis() + ms;
    starttime = millis();

    k = (float)((tempbk * 1.0) / ms);

    // DEBUGF("Backlight_Gradually Val:%d,ms:%d,k:%f\n", val, ms, k);
    Log.verboseln("Backlight Gradual | Val: %d | Length: %d | Step: %f/ms", val, ms, k);

    adjust = true;
}

void backlight_updata(void *params)
{
    while (1) // in its own thread so its fine
    {
        static uint32_t Millis;
        // static uint16_t count;
        static uint32_t duration;

        if (adjust)
        {
            uint32_t mils = millis();
            uint32_t time_ms = 1;

            if (mils - Millis > time_ms)
            {
                duration = mils - starttime;
                setBacklight((uint16_t)((k * duration) + oldBacklight));
                // Serial.printf("K: %f duration: %d old: %d current: %d\n", _k, duration, _old_Backlight, backlight_get_value());

                if (Millis > endtime)
                {
                    adjust = false;
                }

                Millis = mils;
            }
        }

        delay(2);
    }
}

bool displayPeriodic(EventBits_t event, void *arg)
{
    // delay(lv_task_handler());
    lv_task_handler();
    return true;
}

bool displayWakeup(EventBits_t event, void *arg)
{
    // lv_task_handler();
    return true;
}

bool displayInit(EventBits_t event, void *arg)
{
    // Init Backlight
    pinMode(TFT_LED, OUTPUT);
    ledcSetup(0, 2000, 14);
    ledcAttachPin(TFT_LED, 0);
    ledcWrite(0, 0);

    // Log.verboseln("Backlight Init");

    // Init TFT_eSPI
    tft.begin();
    tft.initDMA();
    tft.setSwapBytes(true); // fix inverted colors

    tft.fillScreen(TFT_BLACK);

    // Log.verboseln("TFT_eSPI Init");

    // Init LVGL
    lv_init();

    lv_tick_set_cb(lvTick);

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_flush_cb(display, (lv_display_flush_cb_t)my_flush_cb);

    static uint16_t buf1[TFT_WIDTH * TFT_HEIGHT / 10];
    static uint16_t buf2[TFT_WIDTH * TFT_HEIGHT / 10];
    lv_display_set_buffers(display, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

#ifdef WAVESHARE_ESP32_LCD
    lv_display_set_rotation(display, LV_DISPLAY_ROTATION_90); // lvgl rotation is counter-clockwise
    tft.setRotation((4 - lv_display_get_rotation(display)) % 4);
#endif

    // Log.verboseln("LVGL Init");

    // Init Touch
    touch.begin();
    // delay(1000);

    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_input_read);

    // Log.verboseln("Touch Init");

    // Init Theme
    lv_theme_t *th = lv_theme_default_init(display,                                                                  /*Use the DPI, size, etc from this display*/
                                           lv_palette_main(LV_PALETTE_BLUE), lv_palette_lighten(LV_PALETTE_BLUE, 4), /*Primary and secondary palette*/
                                           true,                                                                     /*Light or dark mode*/
                                           &Outfit_16);                                                              /*Small, normal, large fonts*/

    lv_display_set_theme(display, th); /*Assign the theme to the display*/

    // Log.verboseln("Theme Init");

    // xTaskCreatePinnedToCore(
    //     lvglHandle,                  /* Function to implement the task */
    //     "lvgl_handle",               /* Name of the task */
    //     LV_MEM_SIZE,             /* Stack size in words */
    //     NULL,                        /* Task input parameter */
    //     0,                           /* Priority of the task */
    //     (TaskHandle_t *)&lvglHandle, /* Task handle. */
    //     0);                          /* Core where the task should run */

    // Log.verboseln("Display Init");

    xTaskCreatePinnedToCore(backlight_updata, "backlight", 1024 * 10, NULL, 2, &backlightHandle, 0);

    // xTaskCreatePinnedToCore([](void *params){
    //     delay(lv_task_handler());
    // }, "lvgl", 1024 * 64, NULL, 0, NULL, 1);

    powermgmRegisterCB(displayWakeup, POWERMGM_WAKEUP, "DisplayWakeup");
    powermgmRegisterCBPrio(displayPeriodic, POWERMGM_LOOP_AWAKE, "DisplayPeriodic", CALL_CB_FIRST);

    return true;
}

void setBacklight(int16_t val)
{
    if (val < 100)
    {
        ledcWrite(0, (uint32_t)((val * val * 0.8192) + 0.5)); // math to keep linear brightness
        bgval = val;
    }
    else
    {
        ledcWrite(0, 0x1FFF); // 100% || 8191
        bgval = 100;
    }
}

void setRotation(lv_display_rotation_t rotation)
{
    lv_display_set_rotation(display, rotation); // lvgl rotation is counter-clockwise
    tft.setRotation((4 - lv_display_get_rotation(display)) % 4);
}

int16_t getBacklight() { return bgval; }

bool displaysetup = powermgmRegisterCBPrio(displayInit, POWERMGM_INIT, "ExampleFunc", CALL_CB_FIRST);