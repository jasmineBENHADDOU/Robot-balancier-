#include <Arduino.h>

const int PWM_G = 25;
const int DIR_G = 26;

const int PWM_D = 27;
const int DIR_D = 14;

void initMoteurs()
{
    pinMode(DIR_G, OUTPUT);
    pinMode(DIR_D, OUTPUT);

    ledcAttach(PWM_G, 1000, 8);   // canal PWM ESP32
    ledcAttach(PWM_D, 1000, 8);
}