#include <Arduino.h>
#include <Wire.h>
#include <sensors_data.hpp>

#define HL_PIN 17
#define HL_FREQ 5000
#define HL_CHANNEL 0
#define HL_RES 8

void setup_headlight()
{
    ledcSetup(HL_CHANNEL, HL_FREQ, HL_RES);
    ledcAttachPin(HL_PIN, HL_CHANNEL);
}

void use_headlight()
{
    sensors_data_t *current_data = get_current_sensors_data();

    if (current_data->external.light < 5) {
        ledcWrite(HL_CHANNEL, 255);
    } else if (current_data->external.light < 8) {
        ledcWrite(HL_CHANNEL, 200);
    } else if (current_data->external.light < 12) {
        ledcWrite(HL_CHANNEL, 100);
    } else {
        ledcWrite(HL_CHANNEL, 0);
    }

    free(current_data);
}