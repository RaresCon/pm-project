#pragma once

#include <stdint.h>

typedef struct button_t {
    uint8_t PIN;
    uint64_t last_press;
    bool pressed;
} button;