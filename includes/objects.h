#pragma once
#define _OBJECT_H_
#define AMMO 6
#define METEORS 6
#define ENEMIES 2
#define ENEMY_AMMO 3
#define ENEMY_KAMIKAZE_MAX 2

#include <stdint.h>

void *stdin, *stdout, *stderr, *errno;
/* Contains Shape of Object */
typedef int objectPointer[10][10];

/*  typdef struct for in-game objects */
typedef struct Object {
    float posX, posY, size, velX, velY;
    int health;
    int is_alive, radius;
    objectPointer objForm;
} Object, Player, Meteor, Missile, Enemy_fighter, EnemyShot, Enemy_Kamikaze, Shield;

Player p;
Shield s;
Missile m_array[AMMO];
Meteor mt_array[METEORS];
Enemy_fighter e_array[ENEMIES];
EnemyShot es_array[ENEMY_AMMO];
Enemy_Kamikaze k_array[ENEMY_KAMIKAZE_MAX];

/** Object Constructors **/
void new_player(void);

Enemy_Kamikaze new_kamikaze_enemy(void);

Missile new_missile(void);

Meteor new_meteor(void);

Enemy_fighter new_enemy_fighter(void);

EnemyShot new_enemyShot(void);

void new_shield(void);

void object_move(Object *o);

int check_collision(Object d, Object t);

int withinScreen(Object *o);

/* === UPDATE === */
void player_update(Object *player, int);

void enemy_updating(Object *e);

/* Updates the object. Toggles the Y-velocity if border hit.  */
void object_update(Object *object);