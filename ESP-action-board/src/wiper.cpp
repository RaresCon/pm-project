#include <Arduino.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <sensors_data.hpp>
#include <console_debug.hpp>

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
    sensors_data_t *current_data = get_current_sensors_data();;

    if (current_data->external.rain >= 45) {
        wiper.write(92);
        free(current_data);
        return;
    }

    ESP_DEBUG("Current rain level: %hu\n", current_data->external.rain);

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