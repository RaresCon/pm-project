#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <sensors.hpp>
#include <esp_now_funcs.hpp>
#include <timers.hpp>
#include <console_debug.hpp>

void setup_prereq()
{
    Serial.begin(115200);
    Wire.begin();
}

void setup() {
    setup_prereq();
    setup_sensors();

    populate_sensors_data();

    setup_acquire_timer();
    setup_wifi();
    setup_esp_now();
}

void loop() {
    if (get_critical_data_flag()) {
        if (get_can_transmit_data()) {
            send_response_data(CRITICAL);
            reset_critical_data_flag();
            reset_can_transmit_data();
        }
        return;
    }

    if (get_last_req_type() == FETCH) {
        send_response_data(GOTCHA);
    }

    if (get_acquire_data_flag()) {
        populate_sensors_data();
    }
}
