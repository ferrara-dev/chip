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

enum INPUT
{
    BTN1 = 0x01,
    BTN2 = 0x02,
    BTN3 = 0x04,
    BTN4 = 0x08,
    SW1  = 0x10,
    SW2  = 0x20,
    SW3  = 0x40,
    SW4  = 0x80
};


static volatile int8_t data = 0;
void input_init(void);
uint8_t input_poll(void);
void input_update(void);
int getsw(void);
int getbtns_all(void);
int getBtn(int);



/************ NOT USED ************
 * Initializes buttons and switches.
 ********************************* /
void init_input() {
    set_btn(1, SET);
    set_btn(2, SET);
    set_btn(3, SET);
    set_btn(4, SET);

    set_sw(1, SET);
    set_sw(2, SET);

}
**********************************/