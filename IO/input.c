
/* input.c */

#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/game.h"
#include "../includes/entity.h"
#include "../includes/interface.h"
#include "../includes/functions.h"
#include "../includes/objects.h"
#include "../includes/timers.h"
/* There needs to be a delay in the menus, otherwise it's hard to select buttons */
#define MENU_DELAY 1000000

//void interface_menu_load_paused(void);

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
    BTN1 = 0x01,
    BTN2 = 0x02,
    BTN3 = 0x04,
    BTN4 = 0x08,
    SW1 = 0x10,
    SW2 = 0x20,
    SW3 = 0x40,
    SW4 = 0x80
};

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

/* As input_poll() will be called continously, data will need to be 
 * collected continously and then reset when acted upon
 */
static volatile int8_t data = 0;

uint8_t input_poll(void) {
    data |= (PORTD >> 4) & 0xFE; /* SW4-SW1, BTN4-BTN3 */
    data |= (PORTF >> 1) & 0x01; /* BTN1 */

    return data;
}



static int count = 0;
void input_update(void) {
    /* State needs to be saved in case the player pauses whilst under */
    /* some kind of boost */
    static int prev_state;

    /* As data can be volatile, save it in a local variable for reading */
    int8_t const val = data;

    /* Set inversion flag */
    invert = val & SW3;
    /* Game instructions */
    if (state == STATE_MENU_TESTLED) {
        if (getsw() == 8) {
            prev_state = state;
            interface_menu_load_paused();
        }

        if (data & 0x1) {

        }
    }
    /* Game instructions */
    if (state == STATE_MENU_TESTLED || state == STATE_PLAYING) {
        if (getsw() == 8) {
            prev_state = state;
            interface_menu_load_paused();
        }

        if (((getbtns_all() >> 2) & 0x1) || ((getbtns_all() >> 3) & 0x3) || ((getbtns_all() & 0x1))) {
            move_ship();
        }

        if (getBtn(3)) {
            reload_missiles();
            for(int i = 0; i<AMMO; i++){
                if(!m_array[i].is_alive) {
                    m_array[i].posX = p.posX;
                    m_array[i].posY = p.posY;
                }
            }
            if(count % 5==0)
                shoot();
        }

        count ++;
        if(count > 10000)
            count = 0;

    }



/* Menu instructions */
    if (state & STATE_MENU) {
        quicksleep(MENU_DELAY);

        if (val & BTN3)

            interface_button_next();

        else if (val & BTN2)

            interface_button_press();

        else if (val & BTN1)

            interface_button_prev();

        if (state == STATE_MENU_PAUSED && !(val & SW4))
            state = prev_state;


    }


/* Reset data for next polling */
    data = 0;
}




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

