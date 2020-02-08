
/* game.c */

#include <stdlib.h>
#include <pic32mx.h>
#include "../includes/game.h"
#include "../includes/icon.h"
#include "../includes/interface.h"
#include "../includes/functions.h"
#include "../includes/peripherals.h"
#include "../includes/button.h"
#include "../includes/objects.h"
#include "../includes/graphics.h"
#include "../includes/input.h"
#include "../includes/timers.h"

//Entity ghosts[GAME_MAX_GHOSTS];
//Entity player;
int state;


void interface_menu_load_paused(void);

int m1_spawned = 0;
int m2_spawned = 0;
static int missileCount = 0;
int ammo;

void move_ship(void) {
    if (getBtn(2) && getBtn(4)) {
        if(p.posY > 5)
            p.posY -= p.velY;
    }

    else if (getBtn(2) && getBtn(1)) {
        if(p.posY < DSP_ROWS - 5)
            p.posY += p.velY;
    }

    if (getbtns_all() == 1 || getbtns_all() == 5) {
        if (p.posX < DSP_COLUMNS - 10) {
            p.posX += p.velX;

        }
    } else if (getbtns_all() == 8 || getbtns_all() == 9) {
        if (p.posX > 10) {
            p.posX -= p.velX;
        }
    }
}

/* Check Collision */
/*  @author Johan Edman & Alex Diaz */

void detect_missile_hit(void) {
    int i, j;
    for (i = 0; i < AMMO; i++) {
        if (mt_array[0].is_alive) {
                if (m_array[i].is_alive)
                    if (check_collision(mt_array[0], m_array[i])) {
                        m_array[i].is_alive = 0;
                        mt_array[0].is_alive = 0;
                        draw(m_array[i], 0);
                        draw(mt_array[0], 0);
                    }
        }
    }
}


/* Spawn missile */
void spawn_missile(void) {
    if (!m_array[missileCount].is_alive)
        m_array[missileCount++].is_alive = 1;
    if (missileCount >= AMMO) {
        missileCount = 0;
    }
}

int cooldown = 0;
int counter = 0;

void shoot(void) {
    if (!cooldown) {
        spawn_missile();
        cooldown = 1;
    } else
        cooldown--;

}

void reload_missiles(void) {
    for (int i = 0; i < AMMO; i++) {
        if (!m_array[i].is_alive) {
            m_array[i].posX = p.posX;
            m_array[i].posY = p.posY;
        }
    }
}

/** Spawn a meteor **/

void spawn_meteor(void){
    mt_array[0] = meteor_new();
}
void game_init(void) {
    int i = 0;
    for (i = 0; i < AMMO; i++)
        m_array[i] = missile_new(p);
    interface_menu_load_main(); /// Load the main menu from interface.c
}

/** Set state for LED test
 */
void LED_test_start(void) {
    state = STATE_MENU_TESTLED;
}

void display_test_start(void) {
    /// Init player ///
    player_new();
    spawn_meteor();  /// Tillfällig för att testa "spawn_meteor()"
    set_tmr2_P(TMR2_PLAY);
    state = STATE_PLAYING;
}

void display_test_tick(void) {
    detect_missile_hit();
    draw(p, 1);
    for (int i = 0; i < AMMO; i++)
        if (m_array[i].is_alive) {
            object_update(&m_array[i]);
        }
    //if(mt.is_alive)
        object_update(&mt_array[0]);
}


void LED_test_tick(void) {
    static int timeoutcount = 0;
    if (IFS(0) & 0x9000) {
        IFSCLR(0) = 0x9000;
        timeoutcount++;
        // display_string(0, itoaconv(getbtns_all())); /// Uppdate and Display pushbtn input once every 0,1 second
        //display_update();
        //if (timeoutcount == 10) {
        //  PORTE = PORTE + 0x1;
        // display_string(3, itoaconv(getsw()));   /// Uppdate and Display switch input once every second
        //  display_update();
        // tick(&mytime);
        //
    }
}




