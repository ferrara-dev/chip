
/* icon.h */

#pragma once

#include <stdint.h>
#include "vector.h"

#define FONT_WIDTH  6
#define FONT_HEIGHT 8

#define FONT_WIDTH_8 8

extern int8_t const font[][FONT_WIDTH];
extern int8_t font8x8[128][8];
char textbuffer[4][16];
