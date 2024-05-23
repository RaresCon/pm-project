#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <sensors_data.hpp>
#include <console_debug.hpp>
#include <control_modes.hpp>
#include <buttons.hpp>

#define FREQ 50
#define WIPER_PIN 12
#define WIPER_BUTTON 33

static Servo wiper;
static control current_control_mode = AUTOMATIC;
static button rev_button = { WIPER_BUTTON, 0, false };

void IRAM_ATTR wiper_button_interrupt() {
    uint64_t button_time = millis();
    if (button_time - rev_button.last_press > 250) {
        rev_button.last_press = button_time;
        current_control_mode = (control)((current_control_mode + 1) % 5);
    }
}

void setup_wiper()
{
    wiper.setPeriodHertz(FREQ);
    wiper.attach(WIPER_PIN, 500, 2400);
    pinMode(WIPER_BUTTON, INPUT_PULLUP);
    attachInterrupt(WIPER_BUTTON, wiper_button_interrupt, FALLING);
}

void use_wiper()
{
    sensors_data_t *current_data;

    Serial.println(current_control_mode);

    if (current_control_mode != AUTOMATIC) {
        switch (current_control_mode) {
            case CTL_OFF:
                wiper.write(92);
                break;
            case CTL_LOW:
                wiper.write(98);
                delay(500);
                wiper.write(90);
                delay(500);
                break;
            case CTL_MID:
                wiper.write(104);
                delay(500);
                wiper.write(84);
                delay(500);
                break;
            case CTL_HIGH:
                wiper.write(110);
                delay(500);
                wiper.write(78);
                delay(500);
                break;
            default:
                break;
        }
        wiper.write(92);
        return;
    }

    current_data = get_current_sensors_data();
    ESP_DEBUG("Current rain level: %hu\n", current_data->external.rain);

    if (current_data->external.rain >= 45) {
        wiper.write(92);
        free(current_data);
        return;
    }

    if (current_data->external.rain < 15) {
        wiper.write(110);
        delay(500);
        wiper.write(78);
        delay(500);
    } else if (current_data->external.rain < 25) {
        wiper.write(104);
        delay(500);
        wiper.write(84);
        delay(500);
    } else if (current_data->external.rain < 45) {
        wiper.write(98);
        delay(500);
        wiper.write(90);
        delay(500);
    }

    wiper.write(92);
    free(current_data);
}

uint8_t get_wiper_control_mode()
{
    return current_control_mode;
}