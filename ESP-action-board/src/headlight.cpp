#include <Arduino.h>
#include <Wire.h>
#include <sensors_data.hpp>
#include <console_debug.hpp>
#include <buttons.hpp>

#define HL_PIN 17
#define HL_BUTTON 4
#define HL_FREQ 5000
#define HL_CHANNEL 7
#define HL_RES 6

#define LEVEL_HIGH 63
#define LEVEL_MID 40
#define LEVEL_LOW 20
#define LEVEL_OFF 0

static button hl_button = { 16, 0, false };

void setup_headlight()
{
    ledcSetup(HL_CHANNEL, HL_FREQ, HL_RES);
    ledcAttachPin(HL_PIN, HL_CHANNEL);
}

void use_headlight()
{
    sensors_data_t *current_data = get_current_sensors_data();

    ESP_DEBUG("Current light level: %hu\n", current_data->external.light);

    if (current_data->external.light < 10) {
        ledcWrite(HL_CHANNEL, LEVEL_HIGH);
    } else if (current_data->external.light < 20) {
        ledcWrite(HL_CHANNEL, LEVEL_MID);
    } else if (current_data->external.light < 30) {
        ledcWrite(HL_CHANNEL, LEVEL_LOW);
    } else {
        ledcWrite(HL_CHANNEL, LEVEL_OFF);
    }

    free(current_data);
}