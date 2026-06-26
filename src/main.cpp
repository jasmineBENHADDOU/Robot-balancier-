#include "accel_gyro.h"
#include <Arduino.h>

const float CONSIGNE = 0.0; // si le robot est droit alors l'angle de référence = 0°

void setup()
{
    initMPU();
    calibrerGyro();
}

void loop()
{
    float angle = readAngleFiltre();

    // erreur = consigne - angle;
    float erreur = CONSIGNE - angle;

    // Afficher l'erreur en temps réel
    Serial.print(CONSIGNE);
    Serial.print(",");
    Serial.print(angle);
    Serial.print(",");
    Serial.println(erreur);

    delay(10);
}
