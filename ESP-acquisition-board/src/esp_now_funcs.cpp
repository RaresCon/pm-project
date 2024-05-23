#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <comm_protocol.hpp>
#include <sensors.hpp>
#include <esp_now_funcs.hpp>
#include <console_debug.hpp>

static uint8_t masterAddress[] = {0xC8, 0x2E, 0x18, 0xF7, 0xF3, 0x60};
static uint8_t last_req = 0;
esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    tr_data_t recv_data;
    memset(&recv_data, 0, sizeof(recv_data));
    ESP_DEBUG("[ESP-NOW] receiving request | %d\n", len);

    if (deserialize_tr_data((uint8_t *)incomingData, &recv_data) != SRDS_OK) {
        printf("[ERROR] SERDES: deserializing recv_data failed\n");
        return;
    }

    last_req = recv_data.type;
}

void setup_wifi()
{
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

void setup_esp_now()
{
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register peer
    memcpy(peerInfo.peer_addr, masterAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_register_recv_cb(OnDataRecv);

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void send_response_data(uint8_t type)
{
    sensors_data_t *sensors_data = get_current_sensors_data();
    uint32_t sensors_data_len, response_len;
    tr_data_t response;
    uint8_t* sensors_data_res = NULL, *data = NULL;
    reset_last_req_type();

    if (serialize_sensor_info(sensors_data, sensors_data_res, &sensors_data_len) != SRDS_OK) {
        printf("[ERROR] SERDES: response_len was not populated\n");
        return;
    }

    sensors_data_res = (uint8_t *)malloc(sensors_data_len);
    if (serialize_sensor_info(sensors_data, sensors_data_res, &sensors_data_len) != SRDS_OK) {
        printf("[ERROR] SERDES: serialize response failed\n");
        return;
    }

    response.type = type;
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

    ESP_DEBUG("[ESP-NOW] sending response | %u\n", response_len);
    esp_now_send(masterAddress, data, response_len);

    free(sensors_data);
    free(sensors_data_res);
    free(response.data);
    free(data);
}

uint8_t get_last_req_type()
{
    return last_req;
}

void reset_last_req_type()
{
    last_req = 0;
}
