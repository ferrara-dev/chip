
/* functions.c */

#include <pic32mx.h>
#include "../includes/functions.h"
#include <stdlib.h>
#include "../includes/game.h"
void *stdin, *stdout, *stderr, *errno;

/* BEGINNING OF COPIED CODE */
void quicksleep(int cyc) {
    int i;
    for (i = cyc; i > 0; i--);
}

uint8_t spi_send_recv(uint8_t data) {
    while (!(SPI2STAT & 0x08));
    SPI2BUF = data;
    while (!(SPI2STAT & 1));
    return SPI2BUF;
}


/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
void tickz(unsigned int *timep) {
    /* Get current value, store locally */
    register unsigned int t = *timep;
    t += 1; /* Increment local copy */

    /* If result was not a valid BCD-coded time, adjust now */

    if ((t & 0x0000000f) >= 0x0000000a) t += 0x00000006;
    if ((t & 0x000000f0) >= 0x00000060) t += 0x000000a0;
    /* Seconds are now OK */

    if ((t & 0x00000f00) >= 0x00000a00) t += 0x00000600;
    if ((t & 0x0000f000) >= 0x00006000) t += 0x0000a000;
    /* Minutes are now OK */

    if ((t & 0x000f0000) >= 0x000a0000) t += 0x00060000;
    if ((t & 0x00ff0000) >= 0x00240000) t += 0x00dc0000;
    /* Hours are now OK */

    if ((t & 0x0f000000) >= 0x0a000000) t += 0x06000000;
    if ((t & 0xf0000000) >= 0xa0000000) t = 0;
    /* Days are now OK */

    *timep = t; /* Store new value */
}

/* Integer to decimal string conversion function
 */
void itodsconv(char *buffer, int x) {
    /* Check negative flag */
    if (x < 0) {
        x = -x;
        *(buffer++) = '-';
    }

    /* Compute length */
    int y = x;
    do {
        ++buffer;
        y /= 10;
    } while (y);
    *(buffer--) = '\0';

    /* Write */
    do {
        *(buffer--) = x % 10 + '0';
        x /= 10;
    } while (x);
}

/* END OF COPIED CODE */



unsigned int seed(void) {
    int n = 8;
    unsigned int seed = n;
    for (; n > 0; n--) {
        /* Start sampling, wait until conversion is done */
        AD1CON1 |= (0x1 << 1);
        while (!(AD1CON1 & (0x1 << 1)))
            seed ^= ADC1BUF0 + TMR2;
        while (!(AD1CON1 & 0x1))
            seed ^= ADC1BUF0 + TMR2;

        /* Get the analog value */
        seed ^= ADC1BUF0 + TMR2;
    }
    return seed;
}

/* Reverse every bit in one byte 
 */
int8_t reverse_byte(int8_t x) {
    int8_t y = 0;

    for (int i = 0; i < 7; ++i) {
        y |= ((x & (1 << i)) >> i) << (7 - i);
    }
    return y;
}

int randint(int min, int max) {
    int r = rand() % (max + 1);
    if (r > min)
        return r;
    return r + min;
}

float randfloat(float min, float max) {
    return (((float) rand() / (float) RAND_MAX)) * (max - min) + min;
}

/********************************
 * Had to write my own function
 * to calculate sqrt.
 * Couldn't get #include<math.h>
 * to work.
 ******************************/
int randomize(int chance) {
    if (randint(1, 100) <= chance)
        return 1;
    else
        return 0;
}

float sqrt(int a) {
    int number = a;
    float temp, sqrt;
    // store the half of the given number e.g from 256 => 128
    sqrt = number / 2;
    temp = 0;

    // Iterate until sqrt is different of temp, that is updated on the loop
    while (sqrt != temp) {
        // initially 0, is updated with the initial value of 128
        // (on second iteration = 65)
        // and so on
        temp = sqrt;

        // Then, replace values (256 / 128 + 128 ) / 2 = 65
        // (on second iteration 34.46923076923077)
        // and so on
        sqrt = (number / temp + temp) / 2;
    }

    return sqrt;
}
/* Calculates the distance between to xy-positions. */
/*  @author Johan Edman */
int dist(float x1, float y1, float x2, float y2) {
    int dx = (int) (x2 - x1) * (x2 - x1);
    int dy = (int) (y2 - y1) * (y2 - y1);

    return (int) sqrt(dx + dy);
}
