
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
#include "../includes/memory.h"

int killCounter;
int state;
int gameTimeSeconds;


int cooldown = 0;
int es_cooldown = 0;
int es_counter = 0;

void interface_menu_load_paused(void);

void tmr3_ISR(void);

int missile_reload_cooldown = 0;
static int meteorCount = 0;

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
                health_display[(int) p.health] = 0;
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

    for (i = 0; i < AMMO; i++) {
        if (e_array[0].is_alive) {
            if (m_array[i].is_alive) {
                if (check_collision(m_array[i], e_array[0])) {
                    m_array[i].is_alive = 0;
                    player_score = player_score + ENEMY_KILL_SCORE;
                    draw(m_array[i], 0);
                    e_array[0].health--;
                    if (e_array[0].health < 1) {
                        e_array[0].is_alive = 0;
                        e_array[0].posY = 32; /// if the enemy is hit -> make it invisible and move outside screen
                        draw(e_array[0], 0);
                    }
                }

            }

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

float missile_target_pos;

/* Spawn missile */
void spawn_missile(void) {
    if (!m_array[missileCount].is_alive) {
        missile_target_pos = m_array[missileCount].posY;
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

/* Spawn missile */
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

        case 'e': {
            if (!enemy_shot_cooldown) {
                es_array[0] = new_enemyShot();
                enemy_shot_cooldown = 1;
            }
            break;
        }
        default:
            return;
    }
}

int mt_spawner = 0;

void spawn_meteor(Meteor *o) {
    if (randomize(meteor_spawn_chance)) {
        if (!o->is_alive) {
            o->posX = 110;
            o->posY = 10;
            o->velX = -0.5;      ///(0.1 * difficulty + 1) * -randfloat(0.5, 1.2);
            o->velY = randfloat(0.0, 0.5);
            o->is_alive = 1;
            meteorCount++;
        }
        if (meteorCount >= METEORS)
            meteorCount = 0;
    }
}

void init_meteors(void) {
    int i;
    for (i = 0; i < METEORS; i++) {
        mt_array[i] = new_meteor();
    }
}

void init_projectiles(void) {
    int i;
    for (i = 0; i < AMMO; i++) {
        m_array[i] = new_missile();
    }

    for (i = 0; i < ENEMY_AMMO; i++) {
        es_array[i] = new_enemyShot();
    }
}

void spawn_enemy(void) {
    if (!e_array[0].is_alive)
        if (randomize(enemy_spawn_chance))
            e_array[0] = new_enemy();
}


void game_init(void) {
    high_scores[0] = first_place;
    high_scores[1] = second_place;
    high_scores[2] = third_place;
    init_projectiles();
    interface_menu_load_main(); /// Load the main menu from interface.c
}

/** Set state for LED test
 */
void LED_test_start(void) {
    state = STATE_MENU_TESTLED;
}


void survival_mode_start(void) {
    /// Init player ///
    new_player();
    p.health = PLAYER_MAX_HEALTH;
    spawn_enemy();  /// Tillfällig för att testa "spawn_enemy()"
    init_meteors();
    for (int i = 0; p.health; i++)
        health_display[i] = 1;
    set_tmr2_P(TMR2_PLAY);
    enableTimer3(31250, 0x1B, 0x111, 1);
    gameTimeSeconds = 0;
    player_score = 0;
    set_led(5,0);
    meteor_spawn_chance = METEOR_SPAWN_CHANCE;
    enemy_spawn_chance = ENEMY_SPAWN_CHANCE;
    state = STATE_PLAYING_SURVIVAL_MODE;
}

int dist(float, float, float, float);

int evading = 0;

void enemy_shoot(void) {
    if (randomize(50)) {
        es_array[es_counter] = new_enemyShot();
        es_counter++;
        enemy_shot_cooldown = 2;
    }
    if (es_counter >= ENEMY_AMMO)
        es_counter = 0;
}

void enemy_uppdate(Enemy *e) {
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

void update_leds(void) {
    int i;
    if (p.health == PLAYER_MAX_HEALTH) {
        for (i = 1; i < 4; i++)
            set_led(i, 1);
    } else if (p.health == 2)
        set_led(3, 0);
    else if (p.health == 1)
        set_led(2, 0);
}

void update_score(void) {
    int highscore_flag;
    if (p.is_alive)
        graphics_print(2, 0, itoaconv(player_score));
    else {
        highscore_flag = 0;
        for(int i = 0; i < 3; i++)
        if (high_scores[i] <= player_score) {
            highscore_flag++;
        }

        switch (highscore_flag) {
            case 1:{
                third_place = player_score;
                write_int(TH_ADDR, third_place);
                high_scores[2] = third_place;
                break;
            }

            case 2:{
                third_place = second_place;
                second_place = player_score;
                write_int(SE_ADDR, second_place);
                high_scores[1] = second_place;
                break;
            }

            case 3:{
                third_place = second_place;
                second_place = first_place;
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

void check_player_health(void) {
    update_leds();
    if (p.health < 1) {
        p.is_alive = 0;
        for (int j = 0; j < METEORS; j++)
            mt_array[j].is_alive = 0;
        for (int i = 0; i < AMMO; i++)
            mt_array[i].is_alive = 0;
        for (int k = 0; k < ENEMY_AMMO; k++)
            es_array[k].is_alive = 0;
        update_score();
        e_array[0].is_alive = 0;
        set_tmr2_P(TMR2_MENU);
        interface_menu_load_main();
    }
}

void enemy_updating(Object *e);



void survival_mode_tick(void) {
    draw_borders();
    update_score();
    /** detect if any collision have occured **/
    detect_hit();

    /**check player health **/
    check_player_health();

    /** Update player **/
    draw(p, 1);

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
    if (e_array[0].is_alive) {
        enemy_uppdate(&e_array[0]);
        enemy_updating(&e_array[0]);
    }

    /** Update enemyshots **/
    for (int i = 0; i < ENEMY_AMMO; i++)
        if (es_array[i].is_alive) {
            object_update(&es_array[i]);
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
    if (meteor_spawn_chance < 100)
        meteor_spawn_chance++;
    if (enemy_spawn_chance < 100)
        enemy_spawn_chance++;
}

int showHighscore = 0;


int tog = 0;
int toggle(void){
    tog = ~tog;
    if(!tog)
        return 0;
    if(tog)
        return 1;
}


void tmr3_ISR(void) {
    if (IFS(0) & 0x00100000 && state == STATE_PLAYING_SURVIVAL_MODE) {
        gameTimeSeconds++;
        //  spawn_meteor(&mt_array[meteorCount]);
        if (missile_reload_cooldown) {
            tog = !tog;
            set_led(5,tog);
            set_led(6,tog);
            set_led(7,tog);
            set_led(8,tog);
            missile_reload_cooldown--;
        }

        else{
            set_led(5,0);
            set_led(6,0);
            set_led(7,0);
            set_led(8,0);
        }


        if (enemy_evade_cooldown)
            enemy_evade_cooldown--;

        if (enemy_shot_cooldown)
            enemy_shot_cooldown--;

        if (gameTimeSeconds % 2 == 0) {
            spawn_enemy();
            spawn_meteor(&mt_array[meteorCount]);
        }

        if (gameTimeSeconds % 10 == 0)
            increaseDifficulty();

    }

    IFSCLR(0) = 0x00100000;

}


