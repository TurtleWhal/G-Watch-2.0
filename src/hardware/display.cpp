#include "TFT_eSPI.h"
#include "CST816S.h"
#include "lvgl.h"
#include "display.hpp"
#include <ArduinoLog.h>

TFT_eSPI tft = TFT_eSPI();

lv_display_t *display;
lv_indev_t *indev;

int16_t bgval;

CST816S touch(IIC_SDA, IIC_SCL, TP_RST, TP_INT); // sda, scl, rst, irq

void my_flush_cb(lv_display_t *disp, const lv_area_t *area, void *px_map)
{
    // Serial.printf("Flush cb area: (%i, %i) (%i, %i) Pixel map: %i\n", area->x1, area->y1, area->x2, area->y2, (uint16_t *)px_map);

    uint16_t *buf16 = (uint16_t *)px_map;

    tft.startWrite();
    tft.pushImageDMA(area->x1, area->y1, (area->x2 - area->x1 + 1), (area->y2 - area->y1 + 1), buf16);
    tft.endWrite();

    // for (int j = area->y1; j < area->y2; j++)
    // {
    //     for (int i = area->x1; i < area->x2; i++)
    //     {
    //         Serial.print(buf16[1]);
    //     }
    // }

    lv_display_flush_ready(disp);
}

void my_input_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;

    if (touch.available())
    {

#ifdef WAVESHARE_ESP32_LCD // invert x and y for rotated screen // run after touch.availible because it reads values
        int temp = touch.data.x;
        switch (tft.getRotation())
        {
        default:
            break;
        case 1:
            touch.data.x = touch.data.y;
            touch.data.y = temp;
            break;
        case 2:
            touch.data.x = TFT_WIDTH - touch.data.x;
            touch.data.y = TFT_HEIGHT - touch.data.y;
            break;
        case 3:
            touch.data.x = TFT_HEIGHT - touch.data.y;
            touch.data.y = TFT_WIDTH - temp;
            break;
        }
#endif

        Log.verboseln("Screen Touched at: %d, %d", touch.data.x, touch.data.y);

        last_x = touch.data.x;
        last_y = touch.data.y;
        data->state = LV_INDEV_STATE_PR;
    }
    else
    {
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

void displayInit()
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

#ifdef WAVESHARE_ESP32_LCD
    tft.setRotation(3);
#endif

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

    // Log.verboseln("LVGL Init");

    // Init Touch
    touch.begin();

    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_input_read);

    // Log.verboseln("Touch Init");

    // Init Theme
    lv_theme_t *th = lv_theme_default_init(display,                                                                     /*Use the DPI, size, etc from this display*/
                                           lv_palette_main(LV_PALETTE_ORANGE), lv_palette_main(LV_PALETTE_DEEP_ORANGE), /*Primary and secondary palette*/
                                           true,                                                                        /*Light or dark mode*/
                                           &lv_font_montserrat_14);                                                     /*Small, normal, large fonts*/

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
}

uint32_t displayPeriodic()
{
    return lv_task_handler();
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