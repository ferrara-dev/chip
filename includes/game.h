
/* game.h */

#pragma once

#include "entity.h"
#include "objects.h"

#define GAME_MAX_GHOSTS    6
#define GAME_HIGHSCORES    3
#define GAME_DEFAULT_LIVES 8
#define GAME_BOOST_TICKS   32
#define MISSILE_RELOAD_TIME_S 3
#define PLAYER_MAX_HEALTH 3
#define ENEMY_MAX_HEALTH 2

#define METEOR_SPAWN_CHANCE 50
#define ENEMY_SPAWN_CHANCE  10
#define ENEMY_KILL_SCORE 10
#define METEOR_KILL_SCORE 5

typedef struct level {
    int mt_chance;
    Meteor mt2_array[10];
    Enemy e2_array[10];

} lvl1, lvl2, lvl3;

enum DIFFICULTY {
    DIFFICULTY_EASY = 1,
    DIFFICULTY_MEDIUM = 2,
    DIFFICULTY_HARD = 3,
};


/* Bitmaps are laid out so all menus share the same bit 
 * (e.g. (STATE_MENU_MAIN & STATE_MENU) would evaluate to non-zero)
 * and vice versa for STATE_PLAYING_BOOST_X & STATE_PLAYING
 */
enum STATE {
    STATE_PLAYING_SURVIVAL_MODE = 0x0001,
    STATE_MENU = 0x0002,
    STATE_MENU_MAIN = 0x0006,
    STATE_MENU_HIGHSCORES = 0x000A,
    STATE_MENU_TESTLED = 0x0012,
    STATE_MENU_PAUSED = 0x0022,
    STATE_MENU_HELP = 0x0042,
    STATE_MENU_ADVANCE = 0x0082,
    STATE_MENU_DEAD = 0x0102,
    STATE_MENU_GAMEOVER = 0x0202,
    STATE_PLAYING_BOOST = 0x0401,
    STATE_PLAYING_BOOST_FREEZE = 0x0801,
    STATE_PLAYING_BOOST_SPEED = 0x1401,
    STATE_MENU_WON = 0x2002
};


extern int state;
extern int diff;
extern int highscores[3];

int player_score;
int gameTimeSeconds;
int high_scores[3];

static int shotReady = 1;
static int timeoutcount = 0;
static int missileCount = 0;


int first_place,second_place,third_place;

void LED_test_start(void);

void LED_test_tick(void);

void survival_mode_start(void);

void survival_mode_tick(void);

void game_exit(void);

void game_init(void);

void game_tick(void);

void game_draw(void);

void move_ship(void);

void spawn_missile(void);

void spawn_meteor(Object *o);

void missiles(void);

void shoot(char c);

void reload_missiles(void);

void game_continue(void);

void game_start_easy(void);

void game_start_medium(void);

void game_start_hard(void);

char *difficulty_to_string(void);

int game_score(void);

int game_level(void);

int game_lives(void);

static int meteor_spawn_chance = 0;
/************ Enemy logic **************
  Variabels and functions that control
  enemy ships
 ***************************************/
static int enemy_evade_cooldown = 0;
static int enemy_shot_cooldown = 0;
static int enemy_spawn_chance = 0;
float check_incomming_Missiles(Object *o);
void follow_player(Object *e);
void enemy_evade(float,  Object *e);
void enemy_shoot(void);
