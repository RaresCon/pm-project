#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp1, bmp2;
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

void setup() {
    Serial.begin(115200);
    Wire.begin();
    WiFi.mode(WIFI_MODE_STA);
    Serial.println(WiFi.macAddress());
    analogReadResolution(6);
    pinMode(35, INPUT);
    pinMode(34, INPUT);
    esp_now_peer_info_t peerInfo;

    if(!bmp1.begin(0x76)) {
        Serial.println(bmp1.sensorID());
    }

    if(!bmp2.begin(0x77)) {
        Serial.println(bmp1.sensorID());
    }

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}

void loop() {
    printf("bmp1: %f %f\n", bmp1.readPressure(), bmp1.readTemperature());
    printf("bmp2: %f %f\n", bmp2.readPressure(), bmp2.readTemperature());
    printf("light: %hu\n", analogRead(34));
    printf("rain: %hu\n", analogRead(35));
    delay(2000);
}
