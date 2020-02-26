
/* peripherals.h */

#pragma once

#include <stdint.h>

#define DSP_COLUMNS 128
#define DSP_ROWS    32
#define DSP_PAGES   4
#define TMR2_PLAY 80000000 / (256 * 100)
#define TMR2_MENU 80000000 / (256 * 10)

extern volatile int invert;


void led_init(void);

void led_clear(void);

void led_set(uint8_t);

void set_led(int led, int state);
