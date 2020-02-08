
/* peripherals.h */

#pragma once

#include <stdint.h>
#define DSP_COLUMNS 128
#define DSP_ROWS    32
#define DSP_PAGES   4
#define TMR2_PLAY 80000000 / (256 * 100)
extern volatile int invert;

void    input_init(void);
void    input_update(void);
uint8_t input_poll(void);
void set_tmr2_P(int);
void reset_tmr2_P(void);
void graphics_init(void);
void graphics_set(int, int, int);
void graphics_print(int, int, char const *);
void graphics_reload(void);
void graphics_postprocess(void);
void graphics_clear(void);
void led_init(void);
void led_clear(void);
void led_set(uint8_t);

