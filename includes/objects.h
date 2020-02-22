#pragma once
#define _OBJECT_H_
#define AMMO 6
#define METEORS 6
#define ENEMIES 6
#define ENEMY_AMMO 3
#include <stdint.h>
void *stdin, *stdout, *stderr, *errno;
/* Contains Shape of Object */
typedef int objectPointer[10][10];
typedef int8_t objectPointer2[10];
/*  typdef struct for in-game objects */
typedef struct Object {
    float posX, posY, size, velX, velY;
    int health;
    int is_alive, radius;
    objectPointer objForm;
    objectPointer2 objForm2;
} Object, Player, Meteor, Missile, Enemy, EnemyShot;

Player p;
Missile m_array[AMMO];
Meteor mt_array[METEORS];
Enemy e_array[ENEMIES];
EnemyShot es_array[ENEMY_AMMO];

/** Object Constructors **/
void new_player(void);
Missile new_missile(void);
Meteor new_meteor(void);
Enemy new_enemy(void);
EnemyShot new_enemyShot(void);

void object_move(Object *o);
int check_collision(Object d, Object t);
void within_border(Object *o);
int within_screen(Object *o);
/* === UPDATE === */
void player_update(Object *player);

/* Updates the object. Toggles the Y-velocity if border hit.  */
void object_update(Object *object);