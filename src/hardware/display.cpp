#include "TFT_eSPI.h"
#include "CST816S.h"
#include "lvgl.h"
#include "display.hpp"

TFT_eSPI tft = TFT_eSPI();

lv_display_t *display;
lv_indev_t *indev;

int16_t bgval;

CST816S touch(IIC_SDA, IIC_SCL, 33, 32); // sda, scl, rst, irq

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

    if (touch.available())
    {
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

void initDisplay()
{
    // Init Backlight
    Serial.println("Initializing Backlight");
    pinMode(TFT_LED, OUTPUT);
    ledcSetup(0, 2000, 14);
    ledcAttachPin(TFT_LED, 0);
    ledcWrite(0, 0);

    // Init TFT_eSPI
    tft.begin();
    tft.initDMA();
    tft.setSwapBytes(true); // fix inverted colors

    // Init LVGL
    lv_init();

    display = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
    lv_display_set_flush_cb(display, (lv_display_flush_cb_t)my_flush_cb);

    static uint16_t buf1[TFT_WIDTH * TFT_HEIGHT / 10];
    static uint16_t buf2[TFT_WIDTH * TFT_HEIGHT / 10];
    lv_display_set_buffers(display, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

    // Init Touch
    touch.begin();

    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_input_read);     
}

void displayPeriodic()
{
    static long lastmillis = 0;

    lv_tick_inc(millis() - lastmillis);
    // lv_indev_read(indev);
    lv_timer_handler();
    // touch.available();
    // Serial.println(touch.data.x);

    lastmillis = millis();
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
