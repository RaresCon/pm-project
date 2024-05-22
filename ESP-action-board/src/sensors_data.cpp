#include <Arduino.h>
#include <comm_protocol.hpp>

static sensors_data_t sensors_data;
static bool acquired_sensors_data = false;

void reset_sensors_data()
{
    memset(&sensors_data, 0, sizeof(sensors_data));
}

void set_sensors_data(sensors_data_t *new_data)
{
    memcpy(&sensors_data, new_data, sizeof(sensors_data));
    acquired_sensors_data = true;
}

sensors_data_t *get_current_sensors_data()
{
    sensors_data_t *sensors_data_copy = (sensors_data_t *)malloc(sizeof(*sensors_data_copy));
    memcpy(sensors_data_copy, &sensors_data, sizeof(sensors_data));

    return sensors_data_copy;
}

bool check_sensors_data()
{
    return acquired_sensors_data;
}