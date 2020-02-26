
/* game.h */

#pragma once

#include "entity.h"
#include "objects.h"


#define GAME_HIGHSCORES    3
#define MISSILE_RELOAD_TIME_S 3
#define PLAYER_MAX_HEALTH 3
#define ENEMY_MAX_HEALTH 2
#define MAX_ENEMY_FIGHTERS 2
#define METEOR_SPAWN_CHANCE 40
#define ENEMY_SPAWN_CHANCE  10
#define ENEMY_KILL_SCORE 10
#define METEOR_KILL_SCORE 5
#define KAMIKAZE_KILL_SCORE 10


/* Bitmaps are laid out so all menus share the same bit 
 * (e.g. (STATE_MENU_MAIN & STATE_MENU) would evaluate to non-zero)
 * and vice versa for STATE_PLAYING_BOOST_X & STATE_PLAYING
 */
enum STATE {
    STATE_PLAYING_SURVIVAL_MODE = 0x0001,
    STATE_GAME_OVER = 0x0005,
    STATE_MENU = 0x0002,
    STATE_MENU_MAIN = 0x0006,
    STATE_MENU_HIGHSCORES = 0x000A,

    STATE_MENU_PAUSED = 0x0022,
    STATE_MENU_HELP = 0x0042
};

enum PLAYER_BONUS {
    PIERCING_SHOTS = 0x0001,
    EXTRA_HEALTH   = 0x0002,
};

extern int state;
extern int diff;
extern int highscores[3];
extern int highscore_flag;
extern int player_score;
static int playTime = 0x0000;

int prev_state;
static char textstring[] = "00:00                               ";

char *get_time_string(void);
void reset_playtime(void);

int gameTimeSeconds;
int high_scores[3];

static int missileCount = 0;


int first_place, second_place, third_place;
int meteor_spawn_chance;

void increase_spawn(void);

void time2string(char *, int);

void reset_playTime_vars(void);

void move_ship(void);

void spawn_missile(void);

void spawn_meteor(Object *o);

void missiles(void);

void shoot(char c);

void reload_missiles(void);

int game_over(void);


void LED_test_start(void);

void LED_test_tick(void);

void survival_mode_start(void);

void survival_mode_tick(void);

void end_game(void);
void game_exit(void);

void game_init(void);

void game_tick(void);

void game_draw(void);

/************ Enemy logic **************
  Variabels and functions that control
  enemy ships
 ***************************************/
 int enemy_evade_cooldown;
 int enemy_shot_cooldown;
 int enemy_spawn_chance;
 int kamikaze_spawn_chance;

float check_incomming_Missiles(Object *o);

void follow_player(Object *e);

void enemy_evade(float, Object *e);

void enemy_shoot(void);
