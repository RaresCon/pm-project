#pragma once

typedef enum SCREEN_TYPE {
    TEMP_SCREEN,
    HL_SCREEN,
    WIPER_SCREEN,
    REV_SCREEN,
} SCREEN_TYPE;

void setup_display(void);
void display_current_screen();
bool get_redraw_flag();
void set_redraw_flag();
void inc_current_screen();
uint8_t get_current_screen();