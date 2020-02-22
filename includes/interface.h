
/* interface.h */

#pragma once
#include "game.h"


void interface_draw(void);
void interface_button_prev(void);
void interface_button_next(void);
void interface_button_press(void);

/* This is a very ugly solution, but it allows menu buttons to
 * interact more efficiently using function pointers
 */
void interface_menu_load_main(void);
void interface_menu_load_about(void);
void interface_menu_load_gpioTest(void);
void interface_menu_load_paused(void);
void interface_menu_load_highscores(void);