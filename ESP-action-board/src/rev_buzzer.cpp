#include <Arduino.h>
#include <Wire.h>
#include <comm_protocol.hpp>
#include <sensors_data.hpp>

#define BUZZER_PIN 13

void setup_rev_buzzer()
{
    pinMode(BUZZER_PIN, OUTPUT);
}

void use_buzzer()
{
    sensors_data_t *current_sensors_data = get_current_sensors_data();

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