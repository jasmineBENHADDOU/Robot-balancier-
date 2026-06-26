#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "accel_gyro.h"

Adafruit_MPU6050 mpu;

const float ALPHA = 0.98;
const float ALPHA_GYRO = 0.8;

float gyroX_offset = 0;
float angle_offset = 0;
float angle_filtre = 0;
float gyro_filtre = 0;

unsigned long last_time = 0;

void initMPU()
{
    Serial.begin(115200);
    Wire.begin(21, 22);

    if (!mpu.begin())
    {
        Serial.println("MPU6050 non detecte !");
        while (1)
        {
            delay(10);
        }
    }

    Serial.println("MPU6050 detecte");

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

    last_time = millis();
}

void calibrerGyro()
{
    sensors_event_t accel, gyro, temp;

    float somme_gyro = 0;
    float somme_angle = 0;

    Serial.println("Calibration... garder le robot dans sa position initiale");

    for (int i = 0; i < 500; i++)
    {
        mpu.getEvent(&accel, &gyro, &temp);
        float ay = accel.acceleration.y;
        float az = accel.acceleration.z;

        somme_gyro += gyro.gyro.x * 180.0 / PI;
        somme_angle += atan2(ay, az) * 180.0 / PI;

        delay(2);
    }

    gyroX_offset = somme_gyro / 500.0;
    angle_offset = somme_angle / 500.0;

    angle_filtre = 0;
    gyro_filtre = 0;
    last_time = millis();

    Serial.print("Offset GyroX = ");
    Serial.println(gyroX_offset);

    Serial.print("Angle reference = ");
    Serial.println(angle_offset);
}

float readAngleFiltre()
{
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    float ay = accel.acceleration.y;
    float az = accel.acceleration.z;

    float angle_accel = atan2(ay, az) * 180.0 / PI;
    angle_accel = angle_accel - angle_offset;

    Serial.print("Angle_accel=");
    Serial.println(angle_accel);

    delay(200);

    return angle_accel;
}

/* float readAngleFiltre()
{
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    unsigned long current_time = millis();
    float dt = (current_time - last_time) / 1000.0;
    last_time = current_time;

    if (dt <= 0)
    {
        dt = 0.001;
    }

    float ay = accel.acceleration.y;
    float az = accel.acceleration.z;

    float angle_accel = atan2(ay, az) * 180.0 / PI;
    angle_accel = angle_accel - angle_offset;

    float gyro_x_deg = gyro.gyro.x * 180.0 / PI;
    float gyro_x_corrige = gyro_x_deg - gyroX_offset;

    gyro_filtre = ALPHA_GYRO * gyro_filtre + (1.0 - ALPHA_GYRO) * gyro_x_corrige;

    angle_filtre = ALPHA * (angle_filtre + gyro_filtre * dt) + (1.0 - ALPHA) * angle_accel;

    return angle_filtre;
}