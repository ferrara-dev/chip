//
// Created by Samuel Ferrara on 2020-02-18.
//
#include <stdlib.h>
#include <pic32mx.h>
#include "../includes/icon.h"
#include "../includes/interface.h"
#include "../includes/functions.h"
#include "../includes/peripherals.h"
#include "../includes/button.h"
#include "../includes/objects.h"
#include "../includes/graphics.h"
#include "../includes/input.h"
#include "../includes/timers.h"

int dist(float, float, float, float);
int direction = 0;

/** Detect player missiles within 30 pixel range **/
float check_incomming_Missiles(Object *e) {
    int i, j, distance,react;
    distance = randint(10, 40);
    react = randomize(80);
    for (j = 0; j < AMMO; j++)
        for (i = 0; i < e->radius * 2; i++) {
            if ((e->posY - e->radius + i) == m_array[j].posY && m_array[j].is_alive &&
                dist(m_array[j].posX, m_array[j].posY, e->posX, e->posY) < distance &&
                m_array[j].posX <= e->posX && react) {
                return m_array[j].posY;
            }
        }
    return 0.0;
}

int time = 0;
/** follow the player if no missile is detected **/
void follow_player(Object *e) {
        if (!((e->posY == m_array[missileCount].posY && m_array[missileCount].is_alive))) {
            if (e->posY < p.posY)
                e->posY = e->posY + 0.5;

            else if (e->posY > p.posY)
                e->posY = e->posY - 0.5;
        }
}

/** Evade incoming missile **/
void enemy_evade(float pos, Object *e) {
    int max;
    max = DSP_ROWS - e->size;

    if (pos >= max / 2) {
        direction = 1;
    } else if (pos <= max / 2) {
        direction = -1;
    }
    if (direction == 1 && e->posY < 26) {
        e->velY = -0.5;
    } else if (direction == -1 && e->posY >= 3) {
        e->velY = 0.5;
    }
}

