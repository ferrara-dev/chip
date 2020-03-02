
/* input.c */

#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/game.h"
#include "../includes/menu.h"
#include "../includes/functions.h"
#include "../includes/objects.h"
#include "../includes/timers.h"
#include "../includes/graphics.h"
/* There needs to be a delay in the menus, otherwise it's hard to select buttons */
#define MENU_DELAY 1200000

/** returns value of specified button input value */
int getBtn(int index) {
    switch (index) {
        case 1:
            return (PORTF >> 1) & 0x1;
        case 2:
            return (PORTD >> 5) & 0x1;
        case 3:
            return (PORTD >> 6) & 0x1;
        case 4:
            return (PORTD >> 7) & 0x1;
        default:
            return 0;
    }
    return 0;
}

/** returns current value from chosen switch */
int getSwitch(int index) {
    switch (index) {
        case 1:
            return (PORTD & (0x1 << 8)) >> 8;
        case 2:
            return (PORTD & (0x1 << 9)) >> 9;
        case 3:
            return (PORTD & (0x1 << 10)) >> 10;
        case 4:
            return (PORTD & (0x1 << 11)) >> 11;
        default:
            return 0;
    }
    return 0;
}


void input_init(void) {
    /* PORTD takes SW4-SW1 in bits <11:8> and BTN4-BTN2 in bits <7:5> */
    TRISDSET = 0x7f << 5;
    /* PORTF takes BTN1 in bit <1:1> */
    TRISFSET = 0x01 << 1;
}

int getsw(void) {
    return (PORTD & (0xf << 8)) >> 8;
}

int getbtns_all(void) {
    return (((PORTF >> 1) & 0x1) | ((PORTD & (0x7 << 5)) >> 4));
}


static int count = 0;
static int game_over_count = 200;
static int sc = 0;

void input_update(void) {
    /* State needs to be saved in case the player pauses whilst under */
    /* some kind of boost */
    static int prev_state;

    /*************************** Game instructions *******************************/
    if (state == STATE_PLAYING_SURVIVAL_MODE) {
        if (getSwitch(4)) {
            prev_state = state;
            menu_load_paused();
        }

        if (getSwitch(2) && !game_over() && !getSwitch(4)) {
            graphics_print(2, 3, get_time_string());
        }

        if (getBtn(4) || getBtn(1))
            move_ship();

        if (getBtn(3)) {

            if (count % 5 == 0)
                shoot('p');
        }

        count++;
        if (count > 10000)
            count = 0;

    }

    if (state == STATE_GAME_OVER) {

        if(getBtn(1)){
            graphics_reload();
            graphics_clear();
            menu_load_gameover();
            }
        }

    /*************************** Menu instructions *******************************/
    if (state & STATE_MENU) {
        quicksleep(MENU_DELAY);

        if (getBtn(3))
            menu_label_next();

        else if (getBtn(2))
            menu_label_press();

        else if (getBtn(1))
            menu_label_prev();

        if (state == STATE_MENU_PAUSED && !(getSwitch(4)))
            state = prev_state;


    }


/* Reset data for next polling */
    // data = 0;
}

/** Only used to generate a seed for the rand() function **
 * Initializes analogue pins and turns on ADC.
 * Author: Johan Edman
 **********************************************************/
void init_adc(void) {
    AD1PCFG = 0xFBFF;
    AD1CON1 = 4 << 8 | 7 << 5;
    AD1CHS = 1 << 17;
    TRISBSET = 1 << 11;
    AD1CON2 = 0;
    AD1CON3SET = 1 << 15;

    /* Set up output pins */

    /* Turn on ADC */
    AD1CON1SET = 1 << 15;
}


