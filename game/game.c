
/* game.c */

#include <stdlib.h>
#include <stdint.h>
#include <pic32mx.h>
#include "../includes/game.h"
#include "../includes/icon.h"
#include "../includes/menu.h"
#include "../includes/functions.h"
#include "../includes/button.h"
#include "../includes/objects.h"
#include "../includes/graphics.h"
#include "../includes/input.h"
#include "../includes/timers.h"
#include "../includes/memory.h"
#include "../includes/peripherals.h"

int state;
int gameTimeSeconds;

int cooldown = 0;
int es_cooldown = 0;
int es_counter = 0;

int highscore_flag = 0;
int player_score = 0;
/// Used to control in game difficulty
float difficulty = 0;

void menu_load_paused(void);

void in_game_ISR(void);

int missile_reload_cooldown = 0;
static int meteorCount = 0;

/**
 * TODO
 * Add bonus
 * if 200 points are earned without loosing any health --> bonus
 * 1. Piercing shots for 10 sec - if health = 3
 * 2. Increase health - if health < 3
 *
 *
 */

int bonus_count;
int shield_active;
int kill_streak;
int kill_counter;
void reset_bonus_count(void) {
    bonus_count = 0;
}

void player_bonus(void) {
    if (bonus_count >= 100) {
        if (p.health < PLAYER_MAX_HEALTH) {
            p.health++;
            reset_bonus_count();
        }

        else if (p.health >= PLAYER_MAX_HEALTH) {
            shield_active = 1;
            reset_bonus_count();
        }
    }
}


void move_ship(void) {
    if (state == STATE_PLAYING_SURVIVAL_MODE) {
        if (getBtn(4)) {
            if (p.posY > 5) {
                p.posY -= p.velY;
            }
        }

        if (getBtn(1)) {
            if (p.posY < 20) {
                p.posY += p.velY;
            }

        }

    }
}

int dist(float, float, float, float);

int health_display[3];

/**Detect if objects hit each other**/
void detect_hit(void) {
    int i, j;
    for (i = 0; i < METEORS; i++) {

        if (mt_array[i].is_alive) {
            if (check_collision(p, mt_array[i])) {
                mt_array[i].is_alive = 0;
                mt_array[i].posY = 32; /// if the meteor is hit -> make it invisible and move outside screen
                draw(mt_array[i], 0);
                p.health--;
                reset_bonus_count();
            }
            if (s.is_alive)
                if (check_collision(s, mt_array[i])) {
                    mt_array[i].is_alive = 0;
                    mt_array[i].posY = 32; /// if the meteor is hit -> make it invisible and move outside screen
                    draw(mt_array[i], 0);
                    shield_active = 0;
                }


            for (j = 0; j < AMMO; j++) {
                if (m_array[j].is_alive)
                    if (check_collision(m_array[j], mt_array[i])) {
                        mt_array[i].is_alive = 0;
                        m_array[j].is_alive = 0;
                        mt_array[i].posY = 32; /// if the meteor is hit -> make it invisible and move outside screen
                        draw(m_array[j], 0);
                        draw(mt_array[i], 0);
                        player_score = player_score + METEOR_KILL_SCORE;
                        bonus_count += METEOR_KILL_SCORE;
                    }
            }
        }
    }

    for (i = 0; i < ENEMY_AMMO; i++) {
        if (es_array[i].is_alive) {
            if (check_collision(p, es_array[i])) {
                es_array[i].is_alive = 0;
                draw(es_array[i], 0);
                p.health--;
                reset_bonus_count();
                health_display[(int) p.health] = 0;
            }

            if (s.is_alive)
                if (check_collision(s, es_array[i])) {
                    es_array[i].is_alive = 0;
                    es_array[i].posY = 32; /// if the meteor is hit -> make it invisible and move outside screen
                    draw(es_array[i], 0);
                    shield_active = 0;
                }
            for (j = 0; j < AMMO; j++) {
                if (m_array[j].is_alive)
                    if (check_collision(es_array[i], m_array[j])) {
                        es_array[i].is_alive = 0;
                        m_array[j].is_alive = 0;
                        draw(m_array[j], 0);
                        draw(es_array[i], 0);
                    }

            }
        }
    }
    /// check if enemy fighter ship is hit by missile ///
    for (i = 0; i < AMMO; i++) {
        for (j = 0; j < MAX_ENEMY_FIGHTERS; j++) {
            if (e_array[j].is_alive) {
                if (m_array[i].is_alive) {
                    if (check_collision(m_array[i], e_array[j])) {
                        m_array[i].is_alive = 0;
                        player_score = player_score + ENEMY_KILL_SCORE;
                        draw(m_array[i], 0);
                        e_array[j].health--;
                        bonus_count += ENEMY_KILL_SCORE;
                        if (e_array[j].health < 1) {
                            e_array[j].is_alive = 0;
                            e_array[j].posY = 32; /// if the enemy is hit -> make it invisible and move outside screen
                            draw(e_array[j], 0);
                        }
                    }

                }

            }

        }
    }

    /// check if player is hit by enemy kamikaze ship ///
    for (i = 0; i < ENEMY_KAMIKAZE_MAX; i++) {
        if (k_array[i].is_alive) {
            if (check_collision(p, k_array[i])) {
                k_array[i].is_alive = 0;
                k_array[i].posY = 32;
                draw(k_array[i], 0);
                p.health = p.health - 2;
                reset_bonus_count();
            }

            if (s.is_alive)
                if (check_collision(s, k_array[i])) {
                    k_array[i].is_alive = 0;
                    k_array[i].posY = 32; /// if the meteor is hit -> make it invisible and move outside screen
                    draw(k_array[i], 0);
                    shield_active = 0;
                }
        }
    }
    /// check if enemy kamikaze ship is hit by player missile ///
    for (i = 0; i < ENEMY_KAMIKAZE_MAX; i++)
        if (k_array[i].is_alive)
            for (j = 0; j < AMMO; j++) {
                if (m_array[j].is_alive)
                    if (check_collision(m_array[j], k_array[i])) {
                        k_array[i].is_alive = 0;
                        player_score += KAMIKAZE_KILL_SCORE;
                        bonus_count += KAMIKAZE_KILL_SCORE;
                        k_array[i].posY = 32;
                        draw(k_array[i], 0);
                    }
            }
}

void reload_missiles(void) {
    for (int i = 0; i < AMMO; i++) {
        if (!m_array[i].is_alive) {
            m_array[i].posX = p.posX;
            m_array[i].posY = p.posY;
        }
    }
    missileCount = 0;
    missile_reload_cooldown = MISSILE_RELOAD_TIME_S;
}

/* Spawn missile */
void spawn_missile(void) {
    if (!m_array[missileCount].is_alive) {
        m_array[missileCount++].is_alive = 1;
    }

    if (missileCount >= AMMO) {
        reload_missiles();
    }
}

void reload_enemy_shots(void) {
    for (int i = 0; i < AMMO; i++) {
        if (!m_array[i].is_alive) {
            m_array[i].posX = p.posX;
            m_array[i].posY = p.posY;
        }
    }
    es_counter = 0;
}

/* Spawn enemy shot */
void spawn_enemy_shot(void) {
    if (!es_array[es_counter].is_alive) {
        es_array[es_counter++].is_alive = 1;
    }

    if (es_counter >= ENEMY_AMMO) {
        reload_enemy_shots();
    }
}

void shoot(char c) {
    switch (c) {
        case 'p': {
            if (!missile_reload_cooldown) {
                if (!cooldown) {
                    spawn_missile();
                    cooldown = 1;
                } else
                    cooldown--;
            }
            break;
        }
        default:
            return;
    }
}

/** spawn meteor
 * x-led speed increase with difficulty
 * **/
void spawn_meteor(Meteor *o) {
    int i = 0;
    while (i < METEORS && meteorCount <= METEORS) {
        if (randomize(meteor_spawn_chance)) {
            if (!o->is_alive) {
                o->posX = 110;
                o->posY = randfloat(10, 20);
                o->velX = -1.0 * (1.0 + difficulty) * randfloat(0.5, 0.8);
                if (randomize(50))
                    o->velY = randfloat(0.0, 0.5);
                else
                    o->velY = -1 * randfloat(0.0, 0.5);
                o->is_alive = 1;
                meteorCount++;
            }
        }
        i++;
    }

    if (meteorCount >= METEORS) {
        meteorCount = 0;
        return;
    }
}

/** init the meteors **/
void init_meteors(void) {
    int i;
    for (i = 0; i < METEORS; i++) {
        mt_array[i] = new_meteor();
    }
}

/** init player missiles and enemy shots **/
void init_projectiles(void) {
    int i;
    for (i = 0; i < AMMO; i++) {
        m_array[i] = new_missile();
    }

    for (i = 0; i < ENEMY_AMMO; i++) {
        es_array[i] = new_enemyShot();
    }
}


static int fighter_count = 0;
static int kamikaze_count = 0;
static int kamikaze_spwn_chance = 0;

/***** spawn_enemy(void) *******
 * spawn enemy fighter
 * spawn enemy kamikaze
 ******************************/
void spawn_enemy(void) {
    spawn_fighter:
    if (!e_array[fighter_count].is_alive)
        if (randomize(enemy_spawn_chance)) {
            e_array[fighter_count].posX = 110;
            e_array[fighter_count].posY = (float) randint(8, 22);
            e_array[fighter_count].is_alive = 1;
            e_array[fighter_count].velX = -1.0 * randfloat(0.3, 0.6);
            if (difficulty > 0.5)
                e_array[fighter_count].health = 2;
            fighter_count++;
            if (fighter_count < MAX_ENEMY_FIGHTERS && difficulty > 0.5)
                goto spawn_fighter;
        }


    if (fighter_count >= MAX_ENEMY_FIGHTERS)
        fighter_count = 0;

    if (!k_array[kamikaze_count].is_alive)
        if (randomize(kamikaze_spawn_chance)) {
            k_array[kamikaze_count].posX = 115;
            k_array[kamikaze_count].posY = p.posY;
            k_array[kamikaze_count].is_alive = 1;
            kamikaze_count++;
        }


    if (kamikaze_count >= ENEMY_KAMIKAZE_MAX)
        kamikaze_count = 0;

    return;
}

/** init enemies when game starts **/
void init_enemy(void) {
    for (int i = 0; i < MAX_ENEMY_FIGHTERS; i++)
        e_array[i] = new_enemy_fighter();

    for (int i = 0; i < ENEMY_KAMIKAZE_MAX; i++)
        k_array[i] = new_kamikaze_enemy();
}

void game_init(void) {
    high_scores[0] = first_place;
    high_scores[1] = second_place;
    high_scores[2] = third_place;

    /// Init enemies ///
    init_enemy();
    /// Init meteors ///
    init_meteors();
    /// Init missiles and enemy shots ///
    init_projectiles();

    menu_load_main(); /// Load the main menu from interface.c
}


/*************** survival_mode_start ***************
 * initialization of the game
 * set initial spawn chance for meteors and enemies
 * set initial player health
 * set survival-mode state
 * set gametime to 0
****************************************************/
void survival_mode_start(void) {
    state = STATE_PLAYING_SURVIVAL_MODE;
    graphics_reload();
    graphics_clear();
    /// Init led to display current health ///
    for (int i = 0; p.health; i++)
        health_display[i] = 1;
    reset_playTime_vars();
    /// Init player ///
    new_player();
    new_shield();
    shield_active = 0;
    p.health = PLAYER_MAX_HEALTH;
    enemy_spawn_chance = ENEMY_SPAWN_CHANCE;
    meteor_spawn_chance = METEOR_SPAWN_CHANCE;
    kamikaze_spawn_chance = 0;
    difficulty = 0.0;
    /// Init game time and score ///
    setup_gametime_timers();
    gameTimeSeconds = 0;
    player_score = 0;
    bonus_count = 0;

}

int dist(float, float, float, float);

/** if enemy-fighter is not currently evading incoming missile, 50% chance of shooting at player**/
void enemy_shoot(void) {
    if (randomize(50)) {
        es_array[es_counter] = new_enemyShot();
        es_counter++;
        enemy_shot_cooldown = 2;
    }
    if (es_counter >= ENEMY_AMMO)
        es_counter = 0;
}

int evading = 0;

/** Update enemy fighter ship **/
void enemy_uppdate(Object *e) {
    int offset, side;
    int j, i;
    float estimate_Ypos = 0.0;
    float bottom = e->posY - e->radius;

    if (evading)
        evading--;

    else if (!evading) {
        e->velY = 0;
        if (!enemy_shot_cooldown && e->is_alive) {
            enemy_shoot();
        }

        estimate_Ypos = check_incomming_Missiles(e);
        if (estimate_Ypos)
            if (!enemy_evade_cooldown) {
                enemy_evade_cooldown = 1;
                evading = 17; /// evade for 0,34 seconds
                goto evade;
            }
        follow_player(e);
    }

    return;

    evade:
    enemy_evade(estimate_Ypos, e);
}


int tog = 0;

/******* update_leds(void) ******
 * Update the leds 1-4 to show
 * current health and toggle 5-8
 * when reloading missiles
 *******************************/
void update_leds(char c) {
    int i;
    switch (c) {

        case 'h': {
            if (p.health == PLAYER_MAX_HEALTH) {
                for (i = 1; i < 4; i++)
                    set_led(i, 1);
            } else if (p.health == 2) {
                set_led(3, 0);
                for (i = 1; i < 3; i++)
                    set_led(i, 1);
            } else if (p.health == 1) {
                set_led(3, 0);
                set_led(2, 0);
                set_led(1, 1);
            } else if (p.health < 1) {
                set_led(3, 0);
                set_led(2, 0);
                set_led(1, 0);
            }
            return;
        }

        case 'r': {
            tog = !tog;
            set_led(5, tog);
            set_led(6, tog);
            set_led(7, tog);
            set_led(8, tog);
            return;
        }
        default:
            set_led(5, 0);
            set_led(6, 0);
            set_led(7, 0);
            set_led(8, 0);
            return;
    }
}

/**
  Update the player score
  if player is dead -> check against previous high score
 **/
void update_score(void) {
    if (!game_over())
        graphics_print(2, 0, itoaconv(player_score));
    else {
        highscore_flag = 0;
        for (int i = 0; i < 3; i++)
            if (high_scores[i] < player_score) {
                highscore_flag++;
            }

        switch (highscore_flag) {
            case 1: {
                third_place = player_score;
                write_int(TH_ADDR, third_place);
                high_scores[2] = third_place;
                break;
            }

            case 2: {
                third_place = second_place;
                write_int(TH_ADDR, third_place);
                second_place = player_score;
                write_int(SE_ADDR, second_place);
                high_scores[1] = second_place;
                break;
            }

            case 3: {
                third_place = second_place;
                write_int(TH_ADDR, third_place);
                second_place = first_place;
                write_int(SE_ADDR, second_place);
                first_place = player_score;
                write_int(FI_ADDR, first_place);
                high_scores[0] = first_place;
                break;
            }
            default:
                return;
        }
    }
}


/************ end_game(void) ***************
 * set all objects .is_alive to zero
 * update player score
 * set tmr2 period to menu configuration
 * no return value
 *******************************************/
void end_game(void) {
    update_score();
    reset_playTime_vars();
    p.is_alive = 0;
    disableTimer45();
    set_tmr2_P(TMR2_MENU);
    graphics_reload();
    graphics_clear();

}

/******** check_player_health(void) *********
 * Update leds to show current player health
 * If player health is less than 1 end game
 * no return value
 *******************************************/
void check_player_health(void) {
    update_leds('h');

    if (p.health < 1)
        p.is_alive = 0;
}

void enemy_updating(Object *e);


void survival_mode_tick(void) {
    /**check player health **/
    check_player_health();
    if (!game_over()) {
        draw_borders();
        /** detect if any collision have occured **/
        detect_hit();

        update_score();

        player_bonus();

        /** Update player on the screen **/
        player_update(&p, shield_active);


        /** Update player missiles**/
        for (int i = 0; i < AMMO; i++)
            if (m_array[i].is_alive) {
                object_update(&m_array[i]);
            }

        /** Update Meteors **/
        for (int i = 0; i < METEORS; i++)
            if (mt_array[i].is_alive) {
                object_update(&mt_array[i]);
            }

        /** Update enemyship **/
        for (int i = 0; i < MAX_ENEMY_FIGHTERS; i++)
            if (e_array[i].is_alive) {
                enemy_uppdate(&e_array[i]);
                enemy_updating(&e_array[i]);
            }

        /** Update enemyshots **/
        for (int i = 0; i < ENEMY_AMMO; i++)
            if (es_array[i].is_alive) {
                object_update(&es_array[i]);
            }

        if (k_array[0].is_alive) {
            follow_player(&k_array[0]);
            object_update(&k_array[0]);
        }

    } else {
        end_game();
        state = STATE_GAME_OVER;
    }
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

void increaseDifficulty(void) {
    difficulty += 0.1;
    if (difficulty > 0.5) {

    }
    increase_spawn();
}

/** timer 5 ISR
 * controls spawning of objects, cooldowns, difficulty etc
 * **/

void in_game_ISR(void) {
    if (IFS(0) & 0x00100000 && state == STATE_PLAYING_SURVIVAL_MODE) {
        time2string(get_time_string(), playTime);
        tickz(&playTime);
        gameTimeSeconds++;

        /** toggle led 5 - 8 when missiles are reloading **/
        if (missile_reload_cooldown) {
            update_leds('r');
            missile_reload_cooldown--;
        } else {
            update_leds(' ');
        }

        if (enemy_evade_cooldown)
            enemy_evade_cooldown--;

        if (enemy_shot_cooldown)
            enemy_shot_cooldown--;

        /** attempt to spawn enemy && meteors once every 2 seconds **/
        if (gameTimeSeconds % 2 == 0) {
            spawn_enemy();
            if (difficulty < 0.5)
                spawn_meteor(&mt_array[meteorCount]);
        }
        /** when difficulty gets over 0.5 meteors are spawned every second **/
        if (difficulty >= 0.5)
            spawn_meteor(&mt_array[meteorCount]);

        /** increase the difficulty once every 10 seconds **/
        if (gameTimeSeconds % 10 == 0)
            increaseDifficulty();

    }

    IFSCLR(0) = 0x00100000;
}

/** reset all in-game variables to default values when game ends **/
void reset_playTime_vars(void) {
    playTime = 0x0000;
    time2string(get_time_string(), playTime);
    difficulty = 0;
    meteor_spawn_chance = METEOR_SPAWN_CHANCE;
    enemy_spawn_chance = ENEMY_SPAWN_CHANCE;
    kamikaze_spawn_chance = 0;
    missileCount = 0;
    bonus_count = 0;
    enemy_evade_cooldown = 0;
    missile_reload_cooldown = 0;
    enemy_shot_cooldown = 0;
    for (int j = 0; j < METEORS; j++) {
        mt_array[j].is_alive = 0;
        object_update(&mt_array[j]);
    }
    for (int i = 0; i < AMMO; i++) {
        m_array[i].is_alive = 0;
        object_update(&m_array[i]);
    }
    for (int k = 0; k < ENEMY_AMMO; k++) {
        es_array[k].is_alive = 0;
        object_update(&es_array[k]);
    }
    for (int l = 0; l < MAX_ENEMY_FIGHTERS; l++) {
        e_array[l].is_alive = 0;
        enemy_updating(&e_array[l]);
    }
}

/** when difficulty increase, the chance of meteors and enemies spawning increase **/
void increase_spawn(void) {
    int increase;
    if (meteor_spawn_chance < 100) {
        increase = (int) (difficulty * METEOR_SPAWN_CHANCE);
        meteor_spawn_chance += increase;
    }

    if (enemy_spawn_chance < 100) {
        increase = (int) (difficulty * ENEMY_SPAWN_CHANCE);
        enemy_spawn_chance = enemy_spawn_chance + increase;
    }

    if (difficulty > 0.5) {
        kamikaze_spawn_chance = 20;
    }
}

/******** game_over(void) *********
 * return 1 if player is not alive
 * return 0 if player is alive
 *********************************/
int game_over(void) {
    if (!p.is_alive)
        return 1;
    else
        return 0;
}

char *get_time_string(void) {
    return textstring;
}
