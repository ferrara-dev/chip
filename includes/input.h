/*******************************************************
                    "input.h"
        Headerfile containing function declarations
        used to read and manipulate input.
*******************************************************/

#pragma once

/* input.c */
/* There needs to be a delay in the menus, otherwise it's hard to select buttons */



static volatile int8_t data = 0;

void input_init(void);
void set_sw4_interrupt(void);
void input_update(void);

int getsw(void);

int getbtns_all(void);

int getBtn(int);

void set_sw_interrupts(int index);

int getSwitch(int index);

void init_adc(void);
