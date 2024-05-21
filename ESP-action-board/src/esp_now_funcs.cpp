#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <comm_protocol.hpp>
#include <sensors_data.hpp>
#include <display.hpp>
#include <rev_buzzer.hpp>

static uint8_t slaveAddress[] = {0x10, 0x06, 0x1C, 0x80, 0x9F, 0x5C};
static esp_now_peer_info_t peerInfo;
static bool should_request_data = false;

void setup_wifi()
{
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    tr_data_t recv_data;
    sensors_data_t sensors_data;
    memset(&recv_data, 0, sizeof(recv_data));
    printf("[ESP-NOW] receiving response from slave | %d\n", len);

    if (deserialize_tr_data((uint8_t *)incomingData, &recv_data) != SRDS_OK) {
        printf("[ERROR] SERDES: deserializing recv_data failed\n");
        return;
    }

    memset(&sensors_data, 0, sizeof(sensors_data));
    if (deserialize_sensor_info(recv_data.data, &sensors_data) != SRDS_OK) {
        printf("[ERROR] SERDES: deserializing sensors_data failed\n");
        return;
    }

    set_sensors_data(&sensors_data);
    set_redraw_flag();

    free(recv_data.data);
    free(sensors_data.bmp_data);
}

void setup_esp_now()
{
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_register_recv_cb(OnDataRecv);

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void send_request_for_data()
{
    uint32_t request_len;
    uint8_t *data = NULL;
    tr_data_t request;
    request.type = FETCH;
    request.data_len = 0;

    if (serialize_tr_data(&request, data, &request_len) != SRDS_OK) {
        printf("[ERROR] SERDES: request_len was not populated\n");
        delay(5000);
        return;
    }

    data = (uint8_t *)malloc(request_len);

    if (serialize_tr_data(&request, data, &request_len) != SRDS_OK) {
        printf("[ERROR] SERDES: serialize request failed\n");
        delay(5000);
        return;
    }

    printf("[ESP-NOW] sending request to slave | %u\n", request_len);
    esp_now_send(slaveAddress, data, request_len);

    free(data);

    should_request_data = false;
}

bool get_request_flag()
{
    return should_request_data;
}

void set_request_flag()
{
    should_request_data = true;
}

uint8_t *get_slave_addr()
{
    return slaveAddress;
}