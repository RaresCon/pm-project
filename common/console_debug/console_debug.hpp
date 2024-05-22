#pragma once

#include <stdio.h>

#define DEBUG 0

#if (DEBUG == 1)
#define ESP_DEBUG(fmt, ...) printf(fmt, __VA_ARGS__);
#else
#define ESP_DEBUG(fmt, ...)
#endif