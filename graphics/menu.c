
/* interface.c */

#include <string.h>
#include <stddef.h>
#include "../includes/menu.h"
#include "../includes/button.h"
#include "../includes/graphics.h"
#include "../includes/game.h"
#include "../includes/icon.h"
#include "../includes/functions.h"

#define BUTTONS 4 /* menu will contain a maximum of 3 pressable s */

static Button buttons[BUTTONS];
static int selected = 1;

int getsw(void);

/**************************************
 * Draw all of the menu buttons,
 * and mark the currently selected one
 *************************************/
void draw_menu(void) {
    /* Draw buttons */
    for (int i = 0; i < BUTTONS; ++i) {
        button_draw(&buttons[i], i);
    }

    /* Draw outline around selected button */
    int const page = selected * 8;

    for (int y = page; y < page + 8; ++y) {
        for (int x = 0; x < DSP_COLUMNS; ++x) {
            if ((x == 0 || x == DSP_COLUMNS - 1) || (y == page || y == page + 7))
                graphics_set(x, y, 1);
        }
    }
}


/***************************************
 * Press the currently selected button,
 * and reset the selected index
 **************************************/
void menu_button_press(void) {
    button_press(&buttons[selected]);
    if (state == STATE_MENU_HIGHSCORES || state == STATE_MENU_HELP)
        selected = 3;
    else if (buttons[selected].function != NULL)
        selected = 1;
}


void menu_button_prev(void) {
    int i = 0;
    if (buttons[selected - 1].function == NULL || --selected < 1)
        while(buttons[++i].function == NULL && i < BUTTONS){
            ++selected;
        }
}


void menu_button_next(void) {
    int temp = selected;
    if (buttons[1 + selected].function == NULL || 1 + selected >= BUTTONS)
        return;
    else
        ++selected;
}

void menu_load_main(void) {

    state = STATE_MENU_MAIN;
    button_cons(&buttons[0], "MAIN MENU", NULL);
    button_cons(&buttons[1], "PLAY GAME", survival_mode_start);
    button_cons(&buttons[2], "HIGHSCORES", menu_load_highscores);
    button_cons(&buttons[3], "ABOUT", menu_load_about);
}


void menu_load_about(void) {
    state = STATE_MENU_HELP;
    button_cons(&buttons[0], "BTN1: MOVE           ", NULL);
    button_cons(&buttons[1], "SW4 : PAUSE          ", NULL);
    button_cons(&buttons[2], "SW2 : SHOW TIME      ", NULL);
    button_cons(&buttons[3], "BACK", menu_load_main);
}

void menu_load_paused(void) {
    state = STATE_MENU_PAUSED;
    button_cons(&buttons[0], get_time_string(), NULL);
    button_cons(&buttons[1], "RETURN TO MAIN MENU", menu_load_main);
    button_cons(&buttons[2], "STATS ", NULL);
    button_cons(&buttons[3], " ", NULL);
}

void menu_load_highscores(void) {
    state = STATE_MENU_HIGHSCORES;

    char buffer[BUTTON_BUFFER_SIZE];
    buffer[1] = '.';
    buffer[2] = ' ';

    int i;
    for (i = 0; i < GAME_HIGHSCORES; ++i) {
        buffer[0] = '1' + i;
        itodsconv(&buffer[3], high_scores[i]);
        button_cons(&buttons[i], buffer, NULL);
    }
    button_cons(&buttons[3], "BACK", menu_load_main);
}



