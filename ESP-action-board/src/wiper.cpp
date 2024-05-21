#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <sensors_data.hpp>

#define FREQ 50
#define WIPER_PIN 12

static Servo wiper;

void setup_wiper()
{
    wiper.setPeriodHertz(FREQ);
    wiper.attach(WIPER_PIN, 500, 2400);
}

void use_wiper()
{
    sensors_data_t *current_data = get_current_sensors_data();

    if (current_data->external.rain > 90) {
        return;
    }

    if (current_data->external.rain < 30) {
        wiper.write(90);
        delay(100);
        wiper.write(0);
    } else if (current_data->external.rain < 60) {
        wiper.write(90);
        delay(300);
        wiper.write(0);
    } else if (current_data->external.rain < 90) {
        wiper.write(90);
        delay(500);
        wiper.write(0);
    }

    free(current_data);
}