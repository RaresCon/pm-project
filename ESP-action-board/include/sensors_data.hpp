#pragma once

#include <comm_protocol.hpp>

void reset_data();
void set_sensors_data(sensors_data_t *new_data);
sensors_data_t *get_current_sensors_data();