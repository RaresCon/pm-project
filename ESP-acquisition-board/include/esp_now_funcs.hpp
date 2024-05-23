#pragma once

#include <stdint.h>

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void setup_wifi();
void setup_esp_now();
void send_response_data(uint8_t type);
uint8_t get_last_req_type();
void reset_last_req_type();
uint8_t *get_master_addr();