#pragma once

#include <comm_protocol.hpp>

void reset_sensors_data();
void set_sensors_data(sensors_data_t *new_data);
sensors_data_t *get_current_sensors_data();
bool check_sensors_data();
void set_critical_state();
void reset_critical_state();
bool get_critical_state();