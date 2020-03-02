
/* functions.h */

#pragma once

#include <stdint.h>
#include <stdlib.h>


void *stdin, *stdout, *stderr, *errno;

/* Sets a seed for the rand() function by using analogue inputs on the device */
unsigned int seed();
uint8_t spi_send_recv(uint8_t);
void quicksleep(int);
void tickz( unsigned int * timep );
void itodsconv(char *, int);
void quicksleep(int);
int randint(int, int);
float randfloat(float min, float max);
int randomize(int chance);
char *get_time_string(void);