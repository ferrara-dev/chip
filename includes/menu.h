
/* interface.h */

#ifndef PROJECT_EDITION5_MENU_H
#define PROJECT_EDITION5_MENU_H



#include <string.h>
#include <stddef.h>
#include "game.h"
#include "button.h"

void draw_menu(void);
void menu_button_prev(void);
void menu_button_next(void);
void menu_button_press(void);

/* This is a very ugly solution, but it allows menu buttons to
 * interact more efficiently using function pointers
 */
void menu_load_main(void);
void menu_load_about(void);
void menu_load_paused(void);
void menu_load_highscores(void);

#endif //PROJECT_EDITION5_MENU_H