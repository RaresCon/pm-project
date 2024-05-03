#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
}

void loop() {

}
