#include "Arduino.h"
#include "ArduinoLog.h"
#include "system.hpp"
#include "powermgm.hpp"
#include "ble.hpp"
#include "settings.hpp"

#ifdef WAVESHARE_ESP32_LCD
#include "SensorQMI8658.hpp"
SensorQMI8658 qmi;

// IMUdata acc;
// IMUdata gyr;
#endif

#ifdef LILYGO_TWATCH_2021
#include "SensorBMA423.hpp"
SensorBMA423 bma;
#endif

bool interruptFlag = false;

uint16_t stepday = 0;
uint32_t bootsteps = 0;
uint32_t steps = 0;

void setFlag(void)
{
    interruptFlag = true;
}

void pedometerEvent()
{
    uint32_t val = qmi.getPedometerCounter();
    // Serial.print("Detected Pedometer event : ");
    // Serial.println(val);

    uint32_t diff = (bootsteps + val) - steps;
    steps = bootsteps + val;

    sysinfo.health.steps = steps;
    settings.putUInt("steps", steps);

    sendBLEf("{t:\"act\", stp:%d, rt:true}", steps);
    sendBLEf("{t:\"act\", stp:%d}", diff);
}

bool imuPeriodic(EventBits_t event, void *arg)
{
    // Serial.printf("\t\t\t\t > %i  %.2f *C\n", qmi.getTimestamp(), qmi.getTemperature_C());
#ifdef WAVESHARE_ESP32_LCD
    if (interruptFlag)
    {
        interruptFlag = false;
        qmi.update();
    }

    if (sysinfo.date.doy != stepday)
    {
        stepday = sysinfo.date.doy;
        steps = 0;
        bootsteps = 0;

        qmi.clearPedometerCounter();
        settings.putUInt("steps", 0);
        settings.putUShort("stepday", stepday);
    }

    // if (qmi.getDataReady())
    // {

    // Log.verboseln("Steps: %i", qmi.getPedometerCounter());
    // Log.verboseln("Interrupt: %i", qmi.getIrqStatus());
    // if (digitalRead(IMU_INT1) == HIGH)
    // {
    //     Log.verboseln("INT1");
    // }
    // if (digitalRead(IMU_INT2) == HIGH)
    // {
    //     Log.verboseln("INT2");
    // }

    // sysinfo.health.steps = qmi.getPedometerCounter();

    // qmi.getGyroscope(gyr.x, gyr.y, gyr.z);
    // qmi.getAccelerometer(acc.x, acc.y, acc.z);

    // sysinfo.imu.gyro.x = gyr.x;
    // sysinfo.imu.gyro.y = gyr.y;
    // sysinfo.imu.gyro.z = gyr.z;

    // sysinfo.imu.acc.x = acc.x;
    // sysinfo.imu.acc.y = acc.y;
    // sysinfo.imu.acc.z = acc.z;
    // }
#endif

#ifdef LILYGO_TWATCH_2021

    // int16_t x = 0, y = 0, z = 0;
    // bma.getAccelerometer(x, y, z);

    // double gx = (double)x / 512;
    // double gy = (double)y / -512;
    // double gz = (double)z / -512;

    // Serial.printf("Gyro: X: %9f, Y: %9f, Z: %9f\n", gx, gy, gz);

    // sysinfo.imu.acc.x = gx;
    // sysinfo.imu.acc.y = gy;
    // sysinfo.imu.acc.z = gz;

    // sysinfo.health.steps = bma.getPedometerCounter();

#endif

    // Serial.print("IMU: ");
    // Serial.print(sysinfo.imu.acc.x);
    // Serial.print(", ");
    // Serial.print(sysinfo.imu.acc.y);
    // Serial.print(", ");
    // Serial.print(sysinfo.imu.acc.z);
    // Serial.print(", ");
    // Serial.print(sysinfo.imu.gyro.x);
    // Serial.print(", ");
    // Serial.print(sysinfo.imu.gyro.y);
    // Serial.print(", ");
    // Serial.print(sysinfo.imu.gyro.z);
    // Serial.print(", ");
    // Serial.println(sysinfo.health.steps);

    return true;
}

bool imuInit(EventBits_t event, void *arg)
{
    bootsteps = settings.getUInt("steps", 0);
    stepday = settings.getUShort("stepday", 0);
    steps = bootsteps;
    sysinfo.health.steps = 0;

#ifdef WAVESHARE_ESP32_LCD
    // pinMode(IMU_INT1, INPUT);
    // pinMode(IMU_INT2, INPUT);

    // bool error = qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IIC_SDA, IIC_SCL);
    bool error = qmi.begin(Wire);

    if (!error)
    {
        Serial.println("Failed to find QMI8658");
    }

    /* Get chip id*/
    Serial.print("Device ID:");
    Serial.println(qmi.getChipID(), HEX);

    // Equipped with acceleration sensor, 2G, ORR62.5HZ
    qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_2G, SensorQMI8658::ACC_ODR_62_5Hz);

    // Enable the accelerometer
    qmi.enableAccelerometer();

    //* Indicates the count of sample batch/window for calculation
    uint16_t ped_sample_cnt = 50; // 50 samples
    //* Indicates the threshold of the valid peak-to-peak detection
    uint16_t ped_fix_peak2peak = 200; // 200mg
    //* Indicates the threshold of the peak detection comparing to average
    uint16_t ped_fix_peak = 100; // 100mg
    //* Indicates the maximum duration (timeout window) for a step.
    //* Reset counting calculation if no peaks detected within this duration.
    uint16_t ped_time_up = 200; // 200 samples 4s
    //* Indicates the minimum duration for a step.
    //* The peaks detected within this duration (quiet time) is ignored.
    uint8_t ped_time_low = 20; // 20 samples
    //*   Indicates the minimum continuous steps to start the valid step counting.
    //*   If the continuously detected steps is lower than this count and timeout,the steps will not be take into account;
    //*   if yes, the detected steps will all be taken into account and counting is started to count every following step before timeout.
    //*   This is useful to screen out the fake steps detected by non-step vibrations
    //*   The timeout duration is defined by ped_time_up.
    uint8_t ped_time_cnt_entry = 10; // 10 steps entry count
    //*   Recommended 0
    uint8_t ped_fix_precision = 0;
    //*   The amount of steps when to update the pedometer output registers.
    uint8_t ped_sig_count = 4; // Every 4 valid steps is detected, update the registers once (added by 4).

    qmi.configPedometer(ped_sample_cnt,
                        ped_fix_peak2peak,
                        ped_fix_peak,
                        ped_time_up,
                        ped_time_low,
                        ped_time_cnt_entry,
                        ped_fix_precision,
                        ped_sig_count);

    // Enable the step counter and enable the interrupt
    if (!qmi.enablePedometer(SensorQMI8658::INTERRUPT_PIN_1))
    {
        Serial.println("Enable pedometer failed!");
    }

    // Set the step counter callback function
    qmi.setPedometerEventCallBack(pedometerEvent);

    attachInterrupt(IMU_INT1, setFlag, CHANGE);

    // qmi.configAccelerometer(
    //     /*
    //      * ACC_RANGE_2G
    //      * ACC_RANGE_4G
    //      * ACC_RANGE_8G
    //      * ACC_RANGE_16G
    //      * */
    //     SensorQMI8658::ACC_RANGE_4G,
    //     /*
    //      * ACC_ODR_1000H
    //      * ACC_ODR_500Hz
    //      * ACC_ODR_250Hz
    //      * ACC_ODR_125Hz
    //      * ACC_ODR_62_5Hz
    //      * ACC_ODR_31_25Hz
    //      * ACC_ODR_LOWPOWER_128Hz
    //      * ACC_ODR_LOWPOWER_21Hz
    //      * ACC_ODR_LOWPOWER_11Hz
    //      * ACC_ODR_LOWPOWER_3H
    //      * */
    //     SensorQMI8658::ACC_ODR_1000Hz,
    //     /*
    //      *  LPF_MODE_0     //2.66% of ODR
    //      *  LPF_MODE_1     //3.63% of ODR
    //      *  LPF_MODE_2     //5.39% of ODR
    //      *  LPF_MODE_3     //13.37% of ODR
    //      * */
    //     SensorQMI8658::LPF_MODE_0,
    //     // selfTest enable
    //     true);

    // qmi.configGyroscope(
    //     /*
    //      * GYR_RANGE_16DPS
    //      * GYR_RANGE_32DPS
    //      * GYR_RANGE_64DPS
    //      * GYR_RANGE_128DPS
    //      * GYR_RANGE_256DPS
    //      * GYR_RANGE_512DPS
    //      * GYR_RANGE_1024DPS
    //      * */
    //     SensorQMI8658::GYR_RANGE_64DPS,
    //     /*
    //      * GYR_ODR_7174_4Hz
    //      * GYR_ODR_3587_2Hz
    //      * GYR_ODR_1793_6Hz
    //      * GYR_ODR_896_8Hz
    //      * GYR_ODR_448_4Hz
    //      * GYR_ODR_224_2Hz
    //      * GYR_ODR_112_1Hz
    //      * GYR_ODR_56_05Hz
    //      * GYR_ODR_28_025H
    //      * */
    //     SensorQMI8658::GYR_ODR_896_8Hz,
    //     /*
    //      *  LPF_MODE_0     //2.66% of ODR
    //      *  LPF_MODE_1     //3.63% of ODR
    //      *  LPF_MODE_2     //5.39% of ODR
    //      *  LPF_MODE_3     //13.37% of ODR
    //      * */
    //     SensorQMI8658::LPF_MODE_3,
    //     // selfTest enable
    //     true);

    // // In 6DOF mode (accelerometer and gyroscope are both enabled),
    // // the output data rate is derived from the nature frequency of gyroscope
    // qmi.enableGyroscope();
    // qmi.enableAccelerometer();

    // qmi.configPedometer(0x007D, 0x00CC, 0x0066, 0x00C8, 0x14, 0x0A, 0, 0x04);
    // qmi.enablePedometer();

    // // qmi.enableINT(SensorQMI8658::IntPin1);
    // // qmi.enableINT(SensorQMI8658::IntPin2);
    // // qmi.enableDataReadyINT();

    // // Print register configuration information
    // qmi.dumpCtrlRegister();
#endif

#ifdef LILYGO_TWATCH_2021

    // attachInterrupt(IMU_INT2, setFlag, RISING);
    return true;

    pinMode(IMU_INT1, INPUT);

    if (!bma.begin(Wire, 0x18, IIC_SDA, IIC_SCL))
    {
        Serial.println("Failed to find BMA423 - check your wiring!");
        return true;
    }

    Serial.println("Init BMA423 Sensor success!");

    // Default 4G, 200HZ
    bma.configAccelerometer();

    // Enable acceleration sensor
    bma.enableAccelerometer();

    // Enable pedometer steps
    // bma.enablePedometer();

    // Emptying the pedometer steps
    // bma.resetPedometer();

    // Enable sensor features
    // bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
    //                       SensorBMA423::FEATURE_ANY_MOTION |
    //                       SensorBMA423::FEATURE_ACTIVITY |
    //                       SensorBMA423::FEATURE_TILT |
    //                       SensorBMA423::FEATURE_WAKEUP,
    //                   true);
    // bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
    //                       SensorBMA423::FEATURE_TILT,
    //                   true);

    // bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR, true);

    // Pedometer interrupt enable
    // bma.enablePedometerIRQ();
    // Tilt interrupt enable
    // bma.enableTiltIRQ();
    // DoubleTap interrupt enable
    // bma.enableWakeupIRQ();
    // Any  motion / no motion interrupt enable
    // bma.enableAnyNoMotionIRQ();
    // Activity interruption enable
    // bma.enableActivityIRQ();
    // Chip interrupt function enable
    // bma.configInterrupt();
#endif

    powermgmRegisterCB(imuPeriodic, POWERMGM_LOOP, "IMUPeriodic");
    return true;
}

bool imusetup = powermgmRegisterCBPrio(imuInit, POWERMGM_INIT, "IMUInit", CALL_CB_LAST);