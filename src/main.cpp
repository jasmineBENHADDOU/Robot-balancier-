
#include "accel_gyro.h"

void setup()
{
    initMPU();
    calibrerGyro();
}

void loop()
{
    float angle = readAngleFiltre();

    // erreur = consigne - angle;
}
