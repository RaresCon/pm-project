#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <esp_now.h>
#include <comm_protocol.hpp>
#include <esp_now_funcs.hpp>
#include <timers.hpp>
#include <display.hpp>
#include <rev_buzzer.hpp>

void setup_prereq()
{
    Serial.begin(115200);
    Wire.begin();
}

void setup() {
    setup_prereq();
    setup_wifi();
    setup_esp_now();
    setup_display();
    setup_request_timer();

    setup_rev_buzzer();
    delay(1000);
}

void loop() {
    if (get_request_flag()) {
        send_request_for_data();
    }
    if (get_redraw_flag()) {
        display_current_screen();
    }

    if (get_current_screen() == REV_SCREEN) {
        use_buzzer();
    }
}
