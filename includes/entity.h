
/* entity.h */

#pragma once

#include <stdint.h>
#include "vector.h"
#include "icon.h"
/**********************
 *     NOT USED:
 *     to be removed
 **********************/
typedef struct Ball {
    int speedX;
    int speedY;
    int ball[12][2]; // ball[pixel][x-cord,y-cord]
} Ball;
