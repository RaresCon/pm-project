#pragma once

#include <stdint.h>

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len);
void setup_wifi();
void setup_esp_now();
void send_request_for_data();
bool get_request_flag();
void set_request_flag();
uint8_t *get_slave_addr();