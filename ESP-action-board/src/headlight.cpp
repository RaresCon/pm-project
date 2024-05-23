#include <Arduino.h>
#include <Wire.h>
#include <sensors_data.hpp>
#include <console_debug.hpp>
#include <buttons.hpp>
#include <control_modes.hpp>

#define HL_PIN 17
#define HL_BUTTON 4
#define HL_FREQ 5000
#define HL_CHANNEL 7
#define HL_RES 6

#define LEVEL_HIGH 63
#define LEVEL_MID 40
#define LEVEL_LOW 20
#define LEVEL_OFF 0

static button hl_button = { HL_BUTTON, 0, false };
static control current_control_mode = AUTOMATIC;

void IRAM_ATTR hl_button_interrupt() {
    uint64_t button_time = millis();
    if (button_time - hl_button.last_press > 250) {
        hl_button.last_press = button_time;
        current_control_mode = (control)((current_control_mode + 1) % 5);
    }
}

void setup_headlight()
{
    ledcSetup(HL_CHANNEL, HL_FREQ, HL_RES);
    ledcAttachPin(HL_PIN, HL_CHANNEL);
    pinMode(HL_BUTTON, INPUT_PULLUP);
    attachInterrupt(HL_BUTTON, hl_button_interrupt, FALLING);
}

void use_headlight()
{
    sensors_data_t *current_data;

    if (current_control_mode != AUTOMATIC) {
        switch (current_control_mode) {
            case CTL_OFF:
                ledcWrite(HL_CHANNEL, LEVEL_OFF);
                break;
            case CTL_LOW:
                ledcWrite(HL_CHANNEL, LEVEL_LOW);
                break;
            case CTL_MID:
                ledcWrite(HL_CHANNEL, LEVEL_MID);
                break;
            case CTL_HIGH:
                ledcWrite(HL_CHANNEL, LEVEL_HIGH);
                break;
            default:
                break;
        }
        return;
    }

    current_data = get_current_sensors_data();
    ESP_DEBUG("Current light level: %hu\n", current_data->external.light);

    if (current_data->external.light >= 30) {
        ledcWrite(HL_CHANNEL, LEVEL_OFF);

        free(current_data);
        return;
    }

    if (current_data->external.light < 10) {
        ledcWrite(HL_CHANNEL, LEVEL_HIGH);
    } else if (current_data->external.light < 20) {
        ledcWrite(HL_CHANNEL, LEVEL_MID);
    } else if (current_data->external.light < 30) {
        ledcWrite(HL_CHANNEL, LEVEL_LOW);
    }

    free(current_data);
}

uint8_t get_hl_control_mode()
{
    return current_control_mode;
}