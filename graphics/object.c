#include "../includes/objects.h"
#include <stdlib.h>


/* === CONSTRUCTORS === */

/* Player constructor */
 void player_new(void) {
    p.posX = 10;
    p.posY = 10;
    p.velX = 1;
    p.velY = 1;
    p.size = 10;
    p.is_alive = 1;
    p.radius = 5;

    const int p_shape[10][10] = {
            {1, 1, 0, 0, 1, 1, 0, 0, 1, 1}, {1, 1, 1, 0, 1, 1, 0, 1, 1, 1},
            {1, 0, 1, 0, 1, 1, 0, 1, 0, 1}, {1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
            {0, 0, 0, 1, 1, 1, 1, 0, 0, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 0, 0},
            {0, 0, 1, 1, 1, 1, 1, 1, 0, 0}, {0, 0, 1, 0, 1, 1, 0, 1, 0, 0},
            {0, 0, 1, 0, 1, 1, 0, 1, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            p.objForm[i][j] = p_shape[i][j];

}

Missile missile_new(Object p) {
    Missile m;
    m.posX = p.posX;
    m.posY = p.posY;
    m.velX = 1;
    m.velY = 0;
    m.size = 10;
    m.is_alive = 1;
    m.radius = 2;

    int m_shape[10][10] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };

    int i, j;
    for (i = 0; i < 10; i++)
        for (j = 0; j < 10; j++)
            m.objForm[i][j] = m_shape[i][j];

    return m;
}

/* Object movement */
/*  @author Johan Edman */
void object_move(Object *o) {
    o->posX += o->velX;
    o->posY += o->velY;
}

/* === UPDATE === */
void draw(Object, int);
/* Player Update - Check if within screen */
/*  @author Johan Edman */
void player_move(void){
    p.posX += p.velX;

}
void player_update(Object *p) {
        object_move(p);
    draw(*p, 1);
}

/* Updates the object. Inverts the Y-velocity if border hit.  */
/* TODO
 * if(!whitinscreen)
 *  reset coordinates
 * */
void object_update(Object *o) {
    draw(*o, 0);
    if(!within_screen(o)){
        o->is_alive = 0;
       // o->posX = p.posX;
       // o->posY = p.posY;
    }
    if (o->is_alive) {
        if ((o->posY < 1) || (o->posY + o->size >= 31))
            o->velY = -1 * o->velY * 0.90;
        if (within_screen(o))
            object_move(o);
        draw(*o, 1);
    }

}

/* Check if within game borders */
/*  @author Johan Edman */
void within_border(Object *o) {
    if (o->posX < 0 || o->posX > 150 || o->posY < 0 || o->posY > 31)
        o->is_alive = 0;
}

/* Check if within screen */
/*  @author Johan Edman */
int within_screen(Object *o) {
    return (o->posX > 0 && o->posX < (128 - o->size) &&
            (o->posY + o->velY) > 0 && (o->posY + o->size + o->velY) < 32);
}

int dist(float, float, float, float);
int check_collision(Object dis, Object dat) {
    return dist(dis.posX, dis.posY, dat.posX, dat.posY) < dis.radius;
}