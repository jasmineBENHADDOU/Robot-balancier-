#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "accel.h"


float angle_filtre = 0;
Adafruit_MPU6050 mpu;

void initAccel() {
  Serial.begin(115200);
  Wire.begin(21, 22); // ESP32 : SDA=21, SCL=22

  if (!mpu.begin()) {
    Serial.println("MPU6050 non détecté !");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 détecté");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

 float readAccel() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float ax = accel.acceleration.x;
  float ay = accel.acceleration.y;
  float az = accel.acceleration.z;


  // Angle d'inclinaison avant/arrière
  float angle = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
  angle_filtre = 0.9 * angle_filtre + 0.1 * angle;



Serial.print(">Angle:");
Serial.println(angle_filtre);


delay(20); // 50 Hz
return 0;

}