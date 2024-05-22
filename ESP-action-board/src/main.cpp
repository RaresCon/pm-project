#include <Arduino.h>
#include <Wire.h>

#include <comm_protocol.hpp>
#include <esp_now_funcs.hpp>
#include <timers.hpp>
#include <display.hpp>
#include <rev_buzzer.hpp>
#include <wiper.hpp>
#include <headlight.hpp>
#include <sensors_data.hpp>

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
    setup_rev_buzzer();
    setup_wiper();
    setup_headlight();
    setup_request_timer();

    while (!check_sensors_data()) {
        if (get_request_flag()) {
            send_request_for_data();
        }
    }
}

void loop() {
    if (get_request_flag()) {
        send_request_for_data();
    }
    if (get_redraw_flag()) {
        display_current_screen();
    }

    use_buzzer();
    use_wiper();
    use_headlight();
}
