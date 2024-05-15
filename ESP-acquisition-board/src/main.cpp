#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp1, bmp2;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
    analogReadResolution(6);
    pinMode(35, INPUT);
    pinMode(34, INPUT);

    if(!bmp1.begin(0x76)) {
        Serial.println(bmp1.sensorID());
    }

    if(!bmp2.begin(0x77)) {
        Serial.println(bmp1.sensorID());
    }
}

void loop() {
    printf("bmp1: %f %f\n", bmp1.readPressure(), bmp1.readTemperature());
    printf("bmp2: %f %f\n", bmp2.readPressure(), bmp2.readTemperature());
    printf("light: %hu\n", analogRead(34));
    printf("rain: %hu\n", analogRead(35));
    delay(2000);
}
