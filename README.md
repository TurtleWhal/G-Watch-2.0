# G-Watch #
This watch is a personal project that I have worked on for fun over the last few years. The goal is to build a bluetooth smartwatch that has all the features that I would ever want on a smartwatch, all in a compact format that I can actually wear around all day.

# Features
* LVGL UI
* Touchscreen
* Bluetooth (Using [Gadgetbridge](https://codeberg.org/Freeyourgadget/Gadgetbridge))
  - Phone Notifications
  - Music Control
* Haptic Vibration Motor
* USB-C Charging
* Step Tracking
* Timer
* Stopwatch
* Clock (Obviously)

# Hardware
When I started this project, the [Lilygo T-Watch 2021][lilygo] was the only option on the market for a round screen + processor combo, but then Waveshare released their [ESP32-S3 Dev Board with 1.28in LCD][waveshare] which after desoldering some connectors is significantly thinner than the Lilygo option due to the more integrated processor.

**Specs (Waveshare)**
* ESP32-S3R2 240MHz SoC with 16MB of flash and 2MB of PSRAM
* GC9A01 1.28in round LCD with CST816S capacitive touchscreen
* QMI8658 6-axis IMU

[waveshare]: https://www.waveshare.com/esp32-s3-touch-lcd-1.28.htm
[lilygo]: https://lilygo.cc/products/t-watch-2021

# Construction
The shell of the watch is entirely 3D-Printed, with the main body printed in PLA and the bands printed out of flexible TPU
