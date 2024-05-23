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

void alloc_sensors_data();
int populate_sensors_data();
sensors_data_t *get_current_sensors_data();

void set_acquire_data_flag();
bool get_acquire_data_flag();

void reset_critical_data_flag();
bool get_critical_data_flag();

void set_can_transmit_data();
void reset_can_transmit_data();
bool get_can_transmit_data();