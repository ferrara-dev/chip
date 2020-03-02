#include "../includes/objects.h"
#include "../includes/functions.h"

#include <stdlib.h>
#include <stdint.h>
#include "../includes/graphics.h"
/** === CONSTRUCTORS === **/
void set_led(int led, int state);

void new_player(void) {
    p.posX = 15;
    p.posY = 10;
    p.velX = 1;
    p.velY = 1;
    p.size = 10;
    p.is_alive = 1;
    p.radius = 5;
    p.health = 3;


    const int p_shape[10][10] = {
            {1, 1, 0, 0, 1, 1, 0, 0, 1, 1},
            {1, 1, 1, 0, 1, 1, 0, 1, 1, 1},
            {1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
            {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
            {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},


    };


    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            p.objForm[i][j] = p_shape[i][j];

}

void new_shield(void) {
    s.posX = p.posX + 2;
    s.posY = p.posY;
    s.size = 10;
    s.health = 1;
    s.radius = 2;
    s.is_alive = 0;
    const int s_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            s.objForm[i][j] = s_shape[i][j];

}

Missile new_missile(void) {
    Missile m;
    m.posX = p.posX;
    m.posY = p.posY;
    m.velX = 1;
    m.velY = 0;
    m.size = 10;
    m.is_alive = 1;
    m.radius = 3;
    m.health = 1;
    int m_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            m.objForm[i][j] = m_shape[i][j];
        }
    return m;
}

Enemy_fighter new_enemy_fighter(void) {
    Enemy_fighter e;
    e.posX = 110;
    e.posY = 40;
    e.velX = -0.3;
    e.velY = 0;
    e.size = 10;
    e.is_alive = 1;
    e.radius = 4;
    e.health = 1;
    const int e_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
            {0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 0, 1, 1, 1, 1, 0, 1, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 1, 1, 0, 1, 1, 0, 1, 1, 0},
            {0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            e.objForm[i][j] = e_shape[i][j];

    return e;
}

EnemyShot new_enemyShot(void) {
    EnemyShot enemyShot;
    enemyShot.posX = e_array[0].posX;
    enemyShot.posY = e_array[0].posY;
    enemyShot.velX = -1;
    enemyShot.velY = 0;
    enemyShot.size = 10;
    enemyShot.is_alive = 1;
    enemyShot.radius = 2;
    int es_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++) {
            enemyShot.objForm[i][j] = es_shape[i][j];
        }
    return enemyShot;
}

Enemy_Kamikaze new_kamikaze_enemy(void) {
    Enemy_Kamikaze k;
    k.posX = 110;
    k.posY = 32;
    k.velX = -2;
    k.velY = 0;
    k.size = 10;
    k.is_alive = 1;
    k.radius = 4;
    k.health = 1;
    const int k_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 1, 0, 0, 1, 1, 0, 0, 1, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            k.objForm[i][j] = k_shape[i][j];

    return k;
}

Meteor new_meteor(void) {
    /** enemies and meteors have a default position outside of display **/
    /** is_alive is set as 0 by default and changed to 1 when spawned**/
    Meteor meteor;
    meteor.size = 10;
    meteor.posX = 110;
    meteor.posY = 36;
    meteor.velX = 0;      ///(0.1 * difficulty + 1) * -randfloat(0.5, 1.2);
    meteor.velY = 0;
    meteor.is_alive = 0;
    meteor.radius = 4;

    // if ((rand() & 100) > 50)
    //meteor.velY = (0.1 * (float)(difficulty) + 1) * -randfloat(-0.5, 0.5);

    const int r_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            meteor.objForm[i][j] = r_shape[i][j];

    return meteor;
}

/* move object */
void object_move(Object *o) {
    o->posX += o->velX;
    o->posY += o->velY;
}

/* === UPDATE === */
void draw(Object, int);

/** Updates the player**/
void player_update(Object *o, int shield) {
    draw(*o, 1);

    for (int i = 0; i < AMMO; i++) {
        if (!m_array[i].is_alive) {
            m_array[i].posX = p.posX;
            m_array[i].posY = p.posY;
        }
    }

    if (shield) {
        s.posY = o->posY;
        s.posX = o->posX + 6;
        s.is_alive = 1;
    } else {
        s.posY = 32;
        s.posX = o->posX + 4;
        s.is_alive = 0;
    }
    draw(s, shield);
}


/** Updates enemies. set the Y-velocity to 0 if border hit.  **/
void enemy_updating(Object *e) {
    draw(*e, 0);
    if (!withinScreen(e)) {
        e->is_alive = 0;
    }

    if (e->is_alive) {
        if ((e->posY < 3) || (e->posY + e->size >= 31)) {
            e->velY = 0;
        }
        object_move(e);
        draw(*e, 1);
    } else {
        draw(*e, 0);
        e->posY = 32;
    }

}

/** Updates the object. Inverts the Y-velocity if border hit.  **/
void object_update(Object *o) {
    draw(*o, 0);
    if (!withinScreen(o)) {
        o->is_alive = 0;
    }
    if (o->is_alive) {
        if ((o->posY < 3) || (o->posY + o->size >= 31))
            // o->velY = 0;
            o->velY = -1 * o->velY * 0.90;
        if (withinScreen(o))
            object_move(o);
        draw(*o, 1);
    } else {
        draw(*o, 0);
        o->posY = 32;
    }

}


/** Check if object is within screen **/
int withinScreen(Object *o) {
    return (o->posX > 0 && o->posX < (128 - o->size) &&
            (o->posY + o->velY) > 0 && (o->posY + o->size + o->velY) < 32);
}

int dist(float, float, float, float);

/** Check if the distance between two objects is less then the smallest radius**/
int check_collision(Object dis, Object dat) {
    if (dis.radius > dat.radius)
        return dist(dis.posX, dis.posY, dat.posX, dat.posY) <= dis.radius;
    else
        return dist(dis.posX, dis.posY, dat.posX, dat.posY) <= dat.radius;
}