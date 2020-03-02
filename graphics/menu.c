
/* interface.c */

#include <string.h>
#include <stddef.h>
#include "../includes/menu.h"
#include "../includes/label.h"
#include "../includes/graphics.h"
#include "../includes/game.h"
#include "../includes/icon.h"
#include "../includes/functions.h"

#define LABELS 4 /* menu will contain a maximum of 3 pressable buttons, the top one is used as a header*/

static Label labels[LABELS];
static int selected = 1;

int getsw(void);

/**************************************
 * Draw all of the menu buttons,
 * and mark the currently selected one
 *************************************/
void draw_menu(void) {
    /* Draw buttons */
    for (int i = 0; i < LABELS; ++i) {
        label_draw(&labels[i], i);
    }
    int const page = selected * 8;

    /* Mark currently selected button with a pointing arrow */
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            graphics_set(2 + x, page + y, menu_arrow[y * 8 + x]);
        }
    }
}



/***************************************
 * Press the currently selected label,
 * and reset the selected index
 **************************************/
void menu_label_press(void) {
    label_press(&labels[selected]);
    selected = 1;
    int i = 1;
    while(i < LABELS){
        if(labels[selected].function == NULL)
            ++selected;
        i++;
    }


}


void menu_label_prev(void) {
    int i = 0;
    if (labels[selected - 1].function == NULL || selected - 1 < 1)
        return;
    else
        --selected;
}


void menu_label_next(void) {
    int temp = selected;
    if (labels[1 + selected].function == NULL || 1 + selected > LABELS)
        return;
    else
        ++selected;
}

void menu_load_main(void) {

    state = STATE_MENU_MAIN;
    label_cons(&labels[0], "MAIN MENU", NULL);
    label_cons(&labels[1], "PLAY GAME", survival_mode_start);
    label_cons(&labels[2], "HIGHSCORES", menu_load_highscores);
    label_cons(&labels[3], "ABOUT", menu_load_about);
}


void menu_load_about(void) {
    state = STATE_MENU_HELP;
    label_cons(&labels[0], "BTN1:MOVE", NULL);
    label_cons(&labels[1], "SW4:PAUSE", NULL);
    label_cons(&labels[2], "SW2:SHOW TIME", NULL);
    label_cons(&labels[3], "BACK", menu_load_main);
}

void menu_load_paused(void) {
    state = STATE_MENU_PAUSED;

    label_cons(&labels[0], get_time_string(), NULL);
    label_cons(&labels[1], " return to main", menu_load_main);
    label_cons(&labels[2], " ", NULL);
    label_cons(&labels[3], " ", NULL);
}

void menu_load_highscores(void) {
    state = STATE_MENU_HIGHSCORES;

    char buffer[LABEL_BUFFER_SIZE];
    buffer[1] = '.';
    buffer[2] = ' ';

    int i;
    for (i = 0; i < GAME_HIGHSCORES; ++i) {
        buffer[0] = '1' + i;
        itodsconv(&buffer[3], high_scores[i]);
        label_cons(&labels[i], buffer, NULL);
    }
    label_cons(&labels[3], "BACK", menu_load_main);
}


void menu_load_gameover(void) {
    state = STATE_MENU_GAMEOVER;
    char buffer[LABEL_BUFFER_SIZE];
    if (highscore_flag)
        strcpy(buffer, "NEW HIGHSCORE!");
    else
        strcpy(buffer, " ");
    label_cons(&labels[0], buffer, NULL);

    char buffer2[LABEL_BUFFER_SIZE];
    buffer2[0] = 'S';
    buffer2[1] = 'C';
    buffer2[2] = 'O';
    buffer2[3] = 'R';
    buffer2[4] = 'E';
    buffer2[5] = ' ';
    itodsconv(&buffer2[6], player_score);
    label_cons(&labels[1], buffer2, NULL);
    label_cons(&labels[2],  " return to main", menu_load_main);
    label_cons(&labels[3], " ", NULL);

}