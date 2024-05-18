#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <comm_protocol.hpp>

static uint8_t masterAddress[] = {0xC8, 0x2E, 0x18, 0xF7, 0xF3, 0x60};
static uint8_t last_req = 0;
esp_now_peer_info_t peerInfo;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    tr_data_t recv_data;
    memset(&recv_data, 0, sizeof(recv_data));
    printf("[ESP-NOW] receiving request | %d\n", len);

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

uint8_t get_last_req_type()
{
    return last_req;
}

void reset_last_req_type()
{
    last_req = 0;
}

uint8_t *get_master_addr()
{
    return masterAddress;
}