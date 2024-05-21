#include <Arduino.h>
#include <Wire.h>
#include <esp_now_funcs.hpp>

hw_timer_t *request_timer = NULL;

void ready_to_request_data()
{
    set_request_flag();
}

void setup_request_timer()
{
    request_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(request_timer, &ready_to_request_data, true);
    timerAlarmWrite(request_timer, 1000000, true);
    timerAlarmEnable(request_timer);
}