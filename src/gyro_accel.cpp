#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "accel_gyro.h"

Adafruit_MPU6050 mpu;
float gyroX_offset = 0;
float angle_filtre = 0;
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
    float somme = 0;
    Serial.println("Calibration gyro... ne pas bouger le robot");
    for (int i = 0; i < 500; i++)
    {
        mpu.getEvent(&accel, &gyro, &temp);
        somme += gyro.gyro.x * 180.0 / PI;
        delay(2);
    }
    gyroX_offset = somme / 500.0;
    Serial.print("Offset GyroX = ");
    Serial.println(gyroX_offset);
    last_time = millis();
}

float readAngleFiltre()
{
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    unsigned long current_time = millis();
    float dt = (current_time - last_time) / 1000.0;
    last_time = current_time;

    float ax = accel.acceleration.x;
    float ay = accel.acceleration.y;
    float az = accel.acceleration.z;

    // ✅ CORRECTION 1 : atan2(ay, az) au lieu de atan2(ax, sqrt(ay*ay + az*az))
    // ❌ AVANT : float angle_accel = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    float angle_accel = atan2(ay, az) * 180.0 / PI;

    float gyro_x_deg = gyro.gyro.x * 180.0 / PI;
    float gyro_x_corrige = gyro_x_deg - gyroX_offset;

    angle_filtre = 0.98 * (angle_filtre + gyro_x_corrige * dt)
                 + 0.02 * angle_accel;

    Serial.print(-30);
    Serial.print(" ");
    Serial.print(30);
    Serial.print(" ");
    Serial.println(angle_filtre);

    // ✅ CORRECTION 2 : delay(50) supprimé d'ici
    // ❌ AVANT : delay(50) ici faussait le calcul de dt
    
    return angle_filtre;
}

