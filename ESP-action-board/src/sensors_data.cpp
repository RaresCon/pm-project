#include <Arduino.h>
#include <comm_protocol.hpp>

sensors_data_t sensors_data;

void reset_data()
{
    memset(&sensors_data, 0, sizeof(sensors_data));
}

void set_sensors_data(sensors_data_t *new_data)
{
    memcpy(&sensors_data, new_data, sizeof(sensors_data));
}

sensors_data_t *get_current_sensors_data()
{
    sensors_data_t *sensors_data_copy = (sensors_data_t *)malloc(sizeof(*sensors_data_copy));
    memcpy(sensors_data_copy, &sensors_data, sizeof(sensors_data));

    return sensors_data_copy;
}