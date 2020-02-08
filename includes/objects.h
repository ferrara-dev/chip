
#define _OBJECT_H_
#define AMMO 6
/* Contains Shape of Object */
/*  @author Johan Edman */
typedef int objectPointer[10][10];

/*  @author Alex Diaz - Tweaked by Johan Edman*/
typedef struct Object {
    float posX, posY, size, velX, velY;
    int is_alive, radius;
    objectPointer objForm;
} Object, Player, Rock, Missile;

/* Player Constructor */
void player_new(void);

Player p;
Missile m1;
Missile m2;
Missile m3;
Missile m4;
Missile m_array[AMMO];
Missile missile_new(Object p);
void object_move(Object *o);
int check_collision(Object d, Object t);
void within_border(Object *o);
int within_screen(Object *o);
/* === UPDATE === */
void player_update(Object *player);

/* Updates the object. Toggles the Y-velocity if border hit.  */
void object_update(Object *object);