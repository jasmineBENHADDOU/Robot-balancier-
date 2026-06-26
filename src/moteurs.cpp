#include <Arduino.h>
#include "moteurs.h"

const int AIN1 = 25; // Moteur gauche
const int AIN2 = 26;

const int BIN1 = 27; // Moteur droit
const int BIN2 = 33;

const int FREQ_PWM = 1000;
const int RESOLUTION_PWM = 10; // 0 à 1023

float vitesse_actuelle = 0.0;

void initMoteurs()
{
    ledcAttach(AIN1, FREQ_PWM, RESOLUTION_PWM);
    ledcAttach(AIN2, FREQ_PWM, RESOLUTION_PWM);
    ledcAttach(BIN1, FREQ_PWM, RESOLUTION_PWM);
    ledcAttach(BIN2, FREQ_PWM, RESOLUTION_PWM);

    stopMoteurs();
}

void stopMoteurs()
{
    ledcWrite(AIN1, 0);
    ledcWrite(AIN2, 0);
    ledcWrite(BIN1, 0);
    ledcWrite(BIN2, 0);

    vitesse_actuelle = 0.0;
}

void moteurs(float vitesse_cible)
{
    vitesse_cible = constrain(vitesse_cible, -90, 90);

    float pas_max = 90.0;

    if (vitesse_cible > vitesse_actuelle)
    {
        vitesse_actuelle = min(vitesse_cible, vitesse_actuelle + pas_max);
    }
    else
    {
        vitesse_actuelle = max(vitesse_cible, vitesse_actuelle - pas_max);
    }

    float v_moteur = vitesse_actuelle;

    if (v_moteur > 0 && v_moteur < 8)
        v_moteur = 8;

    if (v_moteur < 0 && v_moteur > -8)
        v_moteur = -8;

    int pwm = abs(v_moteur) * 1023 / 100;

    if (v_moteur >= 0)
    {
        ledcWrite(AIN1, 0);
        ledcWrite(AIN2, pwm);

        ledcWrite(BIN1, 0);
        ledcWrite(BIN2, pwm);
    }
    else
    {
        ledcWrite(AIN1, pwm);
        ledcWrite(AIN2, 0);

        ledcWrite(BIN1, pwm);
        ledcWrite(BIN2, 0);
    }
}