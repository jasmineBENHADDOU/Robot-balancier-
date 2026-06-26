#include "accel_gyro.h"
#include <Arduino.h>
#include <moteurs.h>


float Kp = 3.0;
float Kd = 0.1;

const float CONSIGNE = 0.0;

float erreur_precedente = 0;
unsigned long temps_precedent_pid = 0;


void setup()
{
    initMPU();
    calibrerGyro();
    initMoteurs();
}

void loop()
{

float angle = readAngleFiltre();

float erreur = CONSIGNE - angle;

unsigned long maintenant = millis();
float dt = (maintenant - temps_precedent_pid) / 1000.0;
temps_precedent_pid = maintenant;

if (dt <= 0) dt = 0.001;

float derivee = (erreur - erreur_precedente) / dt;
erreur_precedente = erreur;

float commande = Kp * erreur + Kd * derivee;
commande = constrain(commande, -80, 80);

if (abs(angle) > 45)
{
    stopMoteurs();
}
else
{
    moteurs(commande);
}
}
