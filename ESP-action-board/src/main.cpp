#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_now.h>
#include <comm_protocol.hpp>

static uint8_t slaveAddress[] = {0x10, 0x06, 0x1C, 0x80, 0x9F, 0x5C};
esp_now_peer_info_t peerInfo;

void setup_prereq()
{
    Serial.begin(115200);
    Wire.begin();
}

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

    printf("rain: %hu\n", sensors_data.external.rain);
    printf("light: %hu\n", sensors_data.external.light);
    printf("dist: %hu\n", sensors_data.external.dist);

    for (int i = 0; i < sensors_data.bmp_num; i++) {
        printf("bmp%hhd: %f %f\n", sensors_data.bmp_data[i].sensor_id,
               sensors_data.bmp_data[i].pres, sensors_data.bmp_data[i].temp);
    }

    free(recv_data.data);
    free(sensors_data.bmp_data);
}

void setup_esp_now()
{
    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register peer
    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_register_recv_cb(OnDataRecv);

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void setup() {
    setup_prereq();
    setup_wifi();
    setup_esp_now();
}

void loop() {
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

    delay(5000);
}
