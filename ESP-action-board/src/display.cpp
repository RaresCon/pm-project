#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <sensors_data.hpp>
#include <display.hpp>

#define TFT_MOSI 23  // SDA Pin on ESP32
#define TFT_SCLK 18  // SCL Pin on ESP32
#define TFT_CS   15  // Chip select control pin
#define TFT_DC    22  // Data Command control pin
#define TFT_RST   21  // Reset pin (could connect to RST pin)

static Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
static bool redraw_display = true;
static bool screen_changed = true;
static uint8_t current_screen = 0;

void setup_display(void) {
    Serial.begin(115200);

    tft.init(240, 240, SPI_MODE3);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_ORANGE);
}

void display_temperature() {
    int16_t cursor_y = 0;

    if (screen_changed) {
        tft.fillScreen(ST77XX_BLACK);
        screen_changed = false;
    }

    sensors_data_t *current_data = get_current_sensors_data();

    tft.setTextSize(2);
    tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);

    tft.setCursor(0, cursor_y);

    for (int i = 0; i < current_data->bmp_num; i++) {
        bmp_sensor_data_t bmp = current_data->bmp_data[i];
        tft.printf("bmp%hhu:\n    temp: %.2f\n    pres:%.2f\n",
                   bmp.sensor_id, bmp.temp, bmp.pres);

        cursor_y += 70;
        tft.setCursor(0, cursor_y);
    }

    free(current_data);
}

void display_headlights() {
    tft.fillScreen(ST77XX_CYAN);

    sensors_data_t *current_data = get_current_sensors_data();
}

void display_wipers() {
    tft.fillScreen(ST77XX_RED);

    sensors_data_t *current_data = get_current_sensors_data();
}

void display_reverse() {
    if (screen_changed) {
        tft.fillScreen(ST77XX_MAGENTA);
        screen_changed = false;
    }

    sensors_data_t *current_data = get_current_sensors_data();
}

void display_current_screen()
{
    uint8_t current_screen_type = current_screen % 4;

    switch (current_screen_type) {
        case TEMP_SCREEN:
            display_temperature();
            break;
        case HL_SCREEN:
            display_headlights();
            break;
        case WIPER_SCREEN:
            display_wipers();
            break;
        case REV_SCREEN:
            display_reverse();
            break;
        default:
            break;
    }

    redraw_display = false;
}

bool get_redraw_flag()
{
    return redraw_display;
}

void set_redraw_flag()
{
    redraw_display = true;
}

void inc_current_screen()
{
    screen_changed = true;
    current_screen++;
}

uint8_t get_current_screen()
{
    return current_screen;
}
