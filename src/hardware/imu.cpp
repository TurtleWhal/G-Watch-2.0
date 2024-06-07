#include "Arduino.h"
#include "ArduinoLog.h"
#include "system.hpp"
#include "powermgm.hpp"

#ifdef WAVESHARE_ESP32_LCD
#include "SensorQMI8658.hpp"
SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;
#endif

#ifdef LILYGO_TWATCH_2021
#include "SensorBMA423.hpp"
SensorBMA423 bma;
#endif

bool imuPeriodic(EventBits_t event, void *arg)
{
    // Serial.printf("\t\t\t\t > %i  %.2f *C\n", qmi.getTimestamp(), qmi.getTemperature_C());
#ifdef WAVESHARE_ESP32_LCD
    if (qmi.getDataReady())
    {

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

        qmi.getGyroscope(gyr.x, gyr.y, gyr.z);
        qmi.getAccelerometer(acc.x, acc.y, acc.z);
        sysinfo.health.steps = qmi.getPedometerCounter();

        sysinfo.imu.gyro.x = gyr.x;
        sysinfo.imu.gyro.y = gyr.y;
        sysinfo.imu.gyro.z = gyr.z;

        sysinfo.imu.acc.x = acc.x;
        sysinfo.imu.acc.y = acc.y;
        sysinfo.imu.acc.z = acc.z;
    }
#endif

#ifdef LILYGO_TWATCH_2021

    // int16_t *raw = new int16_t[3];
    // bma.getAccelRaw(raw);
    // sysinfo.imu.acc.x = raw[0];
    // sysinfo.imu.acc.y = raw[1];
    // sysinfo.imu.acc.z = raw[2];

    int16_t x = 0, y = 0, z = 0;
    bma.getAccelerometer(x, y, z);
    // Serial.print("X:");
    // Serial.print(x);
    // Serial.print(" ");
    // Serial.print("Y:");
    // Serial.print(y);
    // Serial.print(" ");
    // Serial.print("Z:");
    // Serial.print(z);
    // Serial.println();

    sysinfo.health.steps = bma.getPedometerCounter();

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

bool sensorIRQ = false;

void setFlag()
{
    sensorIRQ = true;
}

bool imuInit(EventBits_t event, void *arg)
{
#ifdef WAVESHARE_ESP32_LCD
    pinMode(IMU_INT1, INPUT);
    pinMode(IMU_INT2, INPUT);

    qmi.begin(Wire, QMI8658_L_SLAVE_ADDRESS, IIC_SDA, IIC_SCL);

    qmi.configAccelerometer(
        /*
         * ACC_RANGE_2G
         * ACC_RANGE_4G
         * ACC_RANGE_8G
         * ACC_RANGE_16G
         * */
        SensorQMI8658::ACC_RANGE_4G,
        /*
         * ACC_ODR_1000H
         * ACC_ODR_500Hz
         * ACC_ODR_250Hz
         * ACC_ODR_125Hz
         * ACC_ODR_62_5Hz
         * ACC_ODR_31_25Hz
         * ACC_ODR_LOWPOWER_128Hz
         * ACC_ODR_LOWPOWER_21Hz
         * ACC_ODR_LOWPOWER_11Hz
         * ACC_ODR_LOWPOWER_3H
         * */
        SensorQMI8658::ACC_ODR_1000Hz,
        /*
         *  LPF_MODE_0     //2.66% of ODR
         *  LPF_MODE_1     //3.63% of ODR
         *  LPF_MODE_2     //5.39% of ODR
         *  LPF_MODE_3     //13.37% of ODR
         * */
        SensorQMI8658::LPF_MODE_0,
        // selfTest enable
        true);

    qmi.configGyroscope(
        /*
         * GYR_RANGE_16DPS
         * GYR_RANGE_32DPS
         * GYR_RANGE_64DPS
         * GYR_RANGE_128DPS
         * GYR_RANGE_256DPS
         * GYR_RANGE_512DPS
         * GYR_RANGE_1024DPS
         * */
        SensorQMI8658::GYR_RANGE_64DPS,
        /*
         * GYR_ODR_7174_4Hz
         * GYR_ODR_3587_2Hz
         * GYR_ODR_1793_6Hz
         * GYR_ODR_896_8Hz
         * GYR_ODR_448_4Hz
         * GYR_ODR_224_2Hz
         * GYR_ODR_112_1Hz
         * GYR_ODR_56_05Hz
         * GYR_ODR_28_025H
         * */
        SensorQMI8658::GYR_ODR_896_8Hz,
        /*
         *  LPF_MODE_0     //2.66% of ODR
         *  LPF_MODE_1     //3.63% of ODR
         *  LPF_MODE_2     //5.39% of ODR
         *  LPF_MODE_3     //13.37% of ODR
         * */
        SensorQMI8658::LPF_MODE_3,
        // selfTest enable
        true);

    // In 6DOF mode (accelerometer and gyroscope are both enabled),
    // the output data rate is derived from the nature frequency of gyroscope
    qmi.enableGyroscope();
    qmi.enableAccelerometer();

    qmi.configPedometer(0x007D, 0x00CC, 0x0066, 0x00C8, 0x14, 0x0A, 0, 0x04);
    qmi.enablePedometer();

    // qmi.enableINT(SensorQMI8658::IntPin1);
    // qmi.enableINT(SensorQMI8658::IntPin2);
    // qmi.enableDataReadyINT();

    // Print register configuration information
    qmi.dumpCtrlRegister();
#endif

#ifdef LILYGO_TWATCH_2021

    // pinMode(IMU_INT1, INPUT);
    // attachInterrupt(IMU_INT2, setFlag, RISING);

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
    bma.enablePedometer();

    // Emptying the pedometer steps
    // bma.resetPedometer();

    // Enable sensor features
    // bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
    //                       SensorBMA423::FEATURE_ANY_MOTION |
    //                       SensorBMA423::FEATURE_ACTIVITY |
    //                       SensorBMA423::FEATURE_TILT |
    //                       SensorBMA423::FEATURE_WAKEUP,
    //                   true);
    bma.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
                          SensorBMA423::FEATURE_TILT,
                      true);

    // Pedometer interrupt enable
    bma.enablePedometerIRQ();
    // Tilt interrupt enable
    bma.enableTiltIRQ();
    // DoubleTap interrupt enable
    // bma.enableWakeupIRQ();
    // Any  motion / no motion interrupt enable
    // bma.enableAnyNoMotionIRQ();
    // Activity interruption enable
    // bma.enableActivityIRQ();
    // Chip interrupt function enable
    bma.configInterrupt();
#endif

    powermgmRegisterCB(imuPeriodic, POWERMGM_LOOP, "IMUPeriodic");
    return true;
}

bool imusetup = powermgmRegisterCBPrio(imuInit, POWERMGM_INIT, "IMUInit", CALL_CB_FIRST);