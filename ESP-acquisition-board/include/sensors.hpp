#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <defs.hpp>
#include <comm_protocol.hpp>

void setup_sensors();
uint16_t retrieve_dist();
uint16_t retrieve_rain();
uint16_t retrieve_light();
int populate_sensors_data(sensors_data_t *data);
sensors_data_t *alloc_sensors_data();
void free_sensors_data(sensors_data_t **data);