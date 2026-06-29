#include "accel_gyro.h"
#include <Arduino.h>
#include <moteurs.h>


float Kp = 3.0;
float Kd = 0.1;
float Ki = 0.0;

const float CONSIGNE = 0.0;

float erreur_precedente = 0;
unsigned long temps_precedent_pid = 0;
float integrale = 0.0;

void setup()
{
    Serial.begin(115200);

    Serial.println("===== SETUP =====");

    initMPU();
    calibrerGyro();
    initMoteurs();

    temps_precedent_pid = millis();
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

integrale += erreur * dt;
integrale = constrain(integrale, -50, 50);

float commande = Kp * erreur + Ki * integrale + Kd * derivee;
commande = constrain(commande, -50, 50);

// Affichage de debug

Serial.print(angle);
Serial.print(" ");
Serial.print(erreur);
Serial.print(" ");
Serial.print(commande);
Serial.print(" ");
Serial.println(50);

if (abs(angle) > 45)
{
    stopMoteurs();
}
else
{
    moteurs(commande);
}
}

/* #include "accel.h"

void setup()
{
    initAccel();
}

void loop()
{
    readAccel();
} */