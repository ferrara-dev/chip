/*******************************************************
                    "input.h"
        Headerfile containing function declarations
        used to read and manipulate input.
*******************************************************/

#pragma once

/* input.c */
/* There needs to be a delay in the menus, otherwise it's hard to select buttons */
#define MENU_DELAY 1000000

/* The controls are as follows:
 *      SW3: Toggle inverted display
 * When in menu:
 *      BTN2: Press button
 *      BTN1: Up
 *      BTN3: Down
 * When in game:
 *      SW4: Pause
 *      BTN4: Left
 *      BTN3: Down
 *      BTN2: Up
 *      BTN1: Right
 */

enum INPUT {
    BTN1,
    BTN2,
    BTN3,
    BTN4,
    SW1,
    SW2,
    SW3,
    SW4
};


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
