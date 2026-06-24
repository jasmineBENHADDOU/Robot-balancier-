#ifndef ACCEL_H
#define ACCEL_H
#include <Adafruit_MPU6050.h>

extern Adafruit_MPU6050 mpu;

void initAccel();
float readAccel();

#endif