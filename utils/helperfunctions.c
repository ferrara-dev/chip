
/* functions.c */

#include <pic32mx.h>
#include "../includes/functions.h"
#include <stdlib.h>
void *stdin, *stdout, *stderr, *errno;

/* BEGINNING OF COPIED CODE */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}



uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}

/* END OF COPIED CODE */



/* Integer to decimal string conversion function
 */
void itodsconv(char *buffer, int x)
{
	/* Check negative flag */
	if(x < 0)
	{
		x = -x;
		*(buffer++) = '-';
	}
	
	/* Compute length */
	int y = x;
	do
	{
		++buffer;
		y /= 10;
	}while(y);
	*(buffer--) = '\0';
	
	/* Write */
	do
	{
		*(buffer--) = x % 10 + '0';
		x /= 10;
	}while(x);
}




/* Reverse every bit in one byte 
 */
int8_t reverse_byte(int8_t x)
{
	int8_t y = 0;
	
	for(int i = 0; i < 7; ++i)
	{
		y |= ((x & (1 << i)) >> i) << (7 - i);
	}
	return y;
}

/********************************
 * Had to write my own function
 * to calculate sqrt.
 * Couldn't get #include<math.h>
 * to work.
 */
float sqrt(int a)
{
    int number = a;
    float temp, sqrt;
    // store the half of the given number e.g from 256 => 128
    sqrt = number / 2;
    temp = 0;

    // Iterate until sqrt is different of temp, that is updated on the loop
    while(sqrt != temp){
        // initially 0, is updated with the initial value of 128
        // (on second iteration = 65)
        // and so on
        temp = sqrt;

        // Then, replace values (256 / 128 + 128 ) / 2 = 65
        // (on second iteration 34.46923076923077)
        // and so on
        sqrt = ( number/temp + temp) / 2;
    }

   return sqrt;
}
/* Calculates the distance between to xy-positions. */
/*  @author Johan Edman */
int dist(float x1, float y1, float x2, float y2) {
    int dx = (int)(x2 - x1) * (x2 - x1);
    int dy = (int)(y2 - y1) * (y2 - y1);

    return (int)sqrt(dx + dy);
}
