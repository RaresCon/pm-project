#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <sensors.hpp>
#include <esp_now_funcs.hpp>

sensors_data_t *sensors_data;

void setup_prereq()
{
    Serial.begin(115200);
    Wire.begin();
}

void setup() {
    sensors_data = alloc_sensors_data();
    setup_prereq();
    setup_sensors();
    setup_wifi();
    setup_esp_now();
}

void loop() {
    if (get_last_req_type() == 1) {
        uint32_t sensors_data_len, response_len;
        tr_data_t response;
        uint8_t* sensors_data_res = NULL, *data = NULL;
        reset_last_req_type();
        populate_sensors_data(sensors_data);

        if (serialize_sensor_info(sensors_data, sensors_data_res, &sensors_data_len) != SRDS_OK) {
            printf("[ERROR] SERDES: response_len was not populated\n");
            return;
        }

        sensors_data_res = (uint8_t *)malloc(sensors_data_len);
        if (serialize_sensor_info(sensors_data, sensors_data_res, &sensors_data_len) != SRDS_OK) {
            printf("[ERROR] SERDES: serialize response failed\n");
            return;
        }

        response.type = GOTCHA;
        response.data_len = sensors_data_len;
        response.data = (uint8_t *)malloc(response.data_len);
        memcpy(response.data, sensors_data_res, response.data_len);

        if (serialize_tr_data(&response, data, &response_len) != SRDS_OK) {
            printf("[ERROR] SERDES: request_len was not populated\n");
            delay(5000);
            return;
        }

        data = (uint8_t *)malloc(response_len);

        if (serialize_tr_data(&response, data, &response_len) != SRDS_OK) {
            printf("[ERROR] SERDES: serialize request failed\n");
            delay(5000);
            return;
        }

        printf("[ESP-NOW] sending response | %u\n", response_len);
        esp_now_send(get_master_addr(), data, response_len);
        free(sensors_data_res);
        free(response.data);
        free(data);
    }
}
