#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_ST7789.h>
#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>

#include <sensors_data.hpp>
#include <display.hpp>
#include <buttons.hpp>
#include <headlight.hpp>
#include <wiper.hpp>

#define TFT_MOSI    23  // SDA Pin on ESP32
#define TFT_SCLK    18  // SCL Pin on ESP32
#define TFT_CS      15  // Chip select control pin
#define TFT_DC      22  // Data Command control pin
#define TFT_RST     21  // Reset pin (could connect to RST pin)
#define TFT_BUTTON  16

static Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
static bool redraw_display = true;
static bool screen_changed = true;
static uint8_t current_screen = 0;
static button tft_button = { 16, 0, false };

void IRAM_ATTR tft_button_interrupt() {
    uint64_t button_time = millis();
    if (button_time - tft_button.last_press > 250) {
        tft_button.last_press = button_time;
        current_screen++;
        screen_changed = true;
        redraw_display = true;
    }
}

void setup_display(void) {
    Serial.begin(115200);

    tft.init(240, 240, SPI_MODE3);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_ORANGE);
    pinMode(TFT_BUTTON, INPUT_PULLUP);
    attachInterrupt(TFT_BUTTON, tft_button_interrupt, FALLING);
}

void display_temperature() {
    int16_t cursor_y = 0;

    if (screen_changed) {
        tft.fillScreen(ST77XX_BLACK);
        screen_changed = false;
    }

    sensors_data_t *current_data = get_current_sensors_data();

    tft.setTextSize(2);
    if (get_critical_state()) {
        tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
    } else {
        tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
    }

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
    if (screen_changed) {
        tft.fillScreen(ST77XX_CYAN);
        screen_changed = false;

        tft.setTextSize(3);
        tft.setCursor(40, 70);
        tft.setTextColor(ST77XX_BLACK);
        tft.printf("HEADLIGHT\n");
    }

    tft.fillRect(80, 100, 75, 30, ST77XX_CYAN);

    uint8_t control_mode = get_hl_control_mode();

    tft.setCursor(85, 100);
    tft.setTextColor(ST77XX_BLACK);
    if (control_mode == AUTOMATIC) {
        tft.printf("AUTO");

        return;
    } else if (control_mode == CTL_OFF) {
        tft.printf("OFF");

        return;
    }

    tft.setCursor(80, 100);
    switch (control_mode) {
        case CTL_LOW:
            tft.printf("LOW");
            break;
        case CTL_MID:
            tft.printf("MID");
            break;
        case CTL_HIGH:
            tft.printf("HIGH");
            break;
        default:
            break;
    }
}

void display_wipers() {
    if (screen_changed) {
        tft.fillScreen(ST77XX_RED);
        screen_changed = false;
    }

    uint8_t control_mode = get_hl_control_mode();

    tft.setCursor(80, 120);
    tft.setTextColor(ST77XX_BLACK, ST77XX_CYAN);
    tft.printf("HEADLIGHT\n");

    tft.setCursor(100, 150);
    if (control_mode == AUTOMATIC) {
        tft.printf("AUTO");

        return;
    } else if (control_mode == CTL_OFF) {
        tft.printf("OFF");

        return;
    }

    tft.setCursor(0, 120);
    switch (control_mode) {
        case CTL_LOW:
            tft.printf("LOW");
            break;
        case CTL_MID:
            tft.printf("MID");
            break;
        case CTL_HIGH:
            tft.printf("HIGH");
            break;
        default:
            break;
    }
}

void display_reverse() {
    if (screen_changed) {
        tft.fillScreen(ST77XX_MAGENTA);
        screen_changed = false;

        tft.setCursor(0, 110);
        tft.setTextSize(2);
        tft.setTextColor(ST77XX_GREEN);
        tft.printf("Rear <-> object: ");
    }

    sensors_data_t *current_data = get_current_sensors_data();

    if (current_data->external.dist > 300) {
        current_data->external.dist = 0;
    }

    tft.setCursor(200, 110);
    tft.setTextSize(2);
    tft.fillRect(200, 110, 40, 15, ST77XX_MAGENTA);
    tft.setTextColor(ST77XX_GREEN);
    tft.printf("%hu\n", current_data->external.dist);

    free(current_data);
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

uint8_t get_current_screen()
{
    return current_screen;
}
