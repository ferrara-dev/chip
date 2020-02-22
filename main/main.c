
/* main.c */
#include <stdlib.h>
#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/game.h"
#include "../includes/level.h"
#include "../includes/entity.h"
#include "../includes/interface.h"
#include "../includes/timers.h"
#include "../includes/standard.h"
#include "../includes/memory.h"
#include "../includes/input.h"

#define HS_MAX 999
#define HS_LEN 3



void CN_ISR(void);
void tick(void);
void init(void);
/* This is necessary to enable linking with stdlib */
void *stdout;

/* Initialize everything - this includes starting the timer
 * for the interrupt routine that runs the game ticks, and then
 * constantly poll for input until the game exits
 */
int main(int argc, char **argv)
{
	init();

	while(1)
	{
		/* Let interrupts do the work */
		__asm__ volatile("nop");
	}
	
	return 0;
}


int getbtns_all(void);
void display_update(void);
void tick(void)
{
	static int invoc = 0;
	int previousState = state;
    /* Only run this routine every other invocation */
    if(++invoc & 0x1) {
        input_poll();
        input_update();

        if (state & STATE_MENU) {

            if(!check_tmr2_P(TMR2_MENU)) {
                set_tmr2_P(TMR2_MENU);
            }

            if (state == STATE_MENU_MAIN) {
                PORTE = PORTE & 0x00;
            }

            interface_draw();
        }

        if (state == 0x0012) {
            graphics_clear();

           // graphics_reload();
            LED_test_tick();
        }

        if (state == STATE_PLAYING_SURVIVAL_MODE) {
            if(!check_tmr2_P(TMR2_PLAY))
                set_tmr2_P(TMR2_PLAY);

            survival_mode_tick();
        }
        graphics_postprocess();
        //display_update();
        graphics_reload();
        graphics_clear();
        //display_update();

        if (timeoutcount == 10) {
            timeoutcount = 0;
        }
        //TRISDSET = 0x7f << 5;
        timeoutcount++;
    }
	/* Reset interrupt flag */
	IFS(0) &= ~(1 << 8);
}

void init(void)
{
	/* BEGINNING OF COPIED CODE */
	
	/*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */


	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;

	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;

	/* Input pins for input signals */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);

	SPI2CON = 0;
	SPI2BRG = 4;
	SPI2STATCLR = 0x40; /* SPI2STAT bit SPIROV = 0; */
	SPI2CONSET = 0x40; /* SPI2CON bit CKP = 1; */
	SPI2CONSET = 0x20; /* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x8000; /* SPI2CON bit ON = 1; */
      
	/* END OF COPIED CODE */

    init_adc();
    srand(seed());

    i2c_init();

    if (getSwitch(1)) {
        write_int(FI_ADDR, 0);
        write_int(SE_ADDR, 0);
        write_int(TH_ADDR, 0);
    }

    first_place = read_int(FI_ADDR);
    second_place = read_int(SE_ADDR);
    third_place = read_int(TH_ADDR);

	/* Init peripherals */
	graphics_init();
	input_init();
	led_init();
	
	/* Initialize game */
	game_init();

   //
    enable_tmr2();
    enable_tmr2_interrupt();

    enableMultiVectorMode();


    __asm__ ("ei");	      // Enable interrupts
}
