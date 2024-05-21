#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <sensors_data.hpp>

#define TFT_MOSI 23  // SDA Pin on ESP32
#define TFT_SCLK 18  // SCL Pin on ESP32
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    22  // Data Command control pin
#define TFT_RST   21  // Reset pin (could connect to RST pin)

static Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup_display(void) {
    Serial.begin(115200);

    tft.init(240, 240, SPI_MODE3);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_BLACK);
}

void display_temperature() {
    tft.fillScreen(ST77XX_BLACK);

    sensors_data_t *current_data = get_current_sensors_data();


}
