#include <Arduino.h>
#include <Wire.h>
#include <comm_protocol.hpp>
#include <sensors_data.hpp>
#include <buttons.hpp>

#define REV_BUTTON 15
#define BUZZER_PIN 32

static button rev_button = { 15, 0, true };

void IRAM_ATTR rev_button_interrupt() {
    uint64_t button_time = millis();
    if (button_time - rev_button.last_press > 250) {
        rev_button.last_press = button_time;
        rev_button.pressed = !rev_button.pressed;
    }
}

void setup_rev_buzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);
    pinMode(REV_BUTTON, INPUT_PULLUP);
    attachInterrupt(REV_BUTTON, rev_button_interrupt, FALLING);
}

void use_buzzer()
{
    sensors_data_t *current_sensors_data;

    if (!rev_button.pressed) {
        digitalWrite(BUZZER_PIN, LOW);
        return;
    }

    current_sensors_data = get_current_sensors_data();

    if (current_sensors_data->external.dist < 15) {
        digitalWrite(BUZZER_PIN, HIGH);
    } else if (current_sensors_data->external.dist < 50) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(100);
    } else if (current_sensors_data->external.dist < 70) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(200);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(200);
    } else if (current_sensors_data->external.dist < 150) {
        digitalWrite(BUZZER_PIN, LOW);
        delay(400);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(400);
    } else {
        digitalWrite(BUZZER_PIN, LOW);
    }

    free(current_sensors_data);
}
