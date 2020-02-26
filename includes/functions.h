
/* functions.h */

#pragma once

#include <stdint.h>



uint8_t spi_send_recv(uint8_t);
void quicksleep(int);
void tickz( unsigned int * timep );
void itodsconv(char *, int);
int8_t reverse_byte(int8_t);
void quicksleep(int);
int randint(int, int);
float randfloat(float min, float max);
int randomize(int chance);
char *get_time_string(void);