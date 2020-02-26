
#pragma once

#include <stdint.h>
#define TMR2_PLAY 80000000 / (256 * 100)
#define TMR2_MENU 80000000 / (256 * 10)

void enableMultiVectorMode(void);
void enable_tmr2(void);
void enable_tmr2_interrupt(void);
void enableTimer45(void);
void disableTimer45(void);
void setup_gametime_timers(void);
int check_tmr2_P(int);
void set_tmr2_P(int);
void reset_tmr2_P(void);