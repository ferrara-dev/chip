#include <stdlib.h>
#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/game.h"
#include "../includes/level.h"
#include "../includes/entity.h"
#include "../includes/interface.h"

int tmr2_state;

/**
 * Enables multi vector mode which allows
 * multiple distinct interrupt handlers.
 */
void enableMultiVectorMode(void) {
    INTCONSET = 0x1000;
}

void enable_tmr2(void) {
    /* Setup timer used to update display every 0.1 seconds */
    T2CONCLR = 1 << 15;	  // Disable timer
    T2CONSET = 7 << 4;	   // Prescaler = 256
    PR2 = 80000000 / (256 * 10); // Tick every 0.1 seconds
    TMR2 = 0;		    // Reset timer value
    T2CONSET = 1 << 15;	  // Enable timer
}

void set_tmr2_P(int p){
    __asm__ volatile("di");
    T2CONCLR = 1 << 15;	  // Disable timer
    T2CONSET = 7 << 4;	   // Prescaler = 256
    PR2 = p;
    TMR2 = 0;		    // Reset timer value
    T2CONSET = 1 << 15;	  // Enable timer
    tmr2_state = p;
    __asm__ volatile("ei");
}
int check_tmr2_P(int period){
    if(tmr2_state == period)
        return 1;
    else
        return 0;
}
void enable_tmr2_interrupt(void) {
    IEC(0) |= 1 << 8;	    // Set interrupt 2 to listen to timer 2
    IPC(2) |= 7 << 0;
    //IPCSET(2) = 0xD;// Set interrupt 2 priority to 7 (highest)
}

/**
 *  Set up and start timer 3
 */
void enableTimer3(int period, int priority, int prescaling, int interrupts) {
    T4CON = 0x0; // Stop any 16/32-bit Timer4 operation
    T5CON = 0x0; // Stop any 16-bit Timer5 operation
    T4CONSET = 0x0038; // Enable 32-bit mode, prescaler 1:8,
    // internal peripheral clock source
    TMR4 = 0x0; // Clear contents of the TMR4 and TMR5
    PR4 = 80000000 / (8); // Load PR4 and PR5 registers with 32-bit value
   // T4CONSET = 0x8000; // Start Timer4/5

    IPCSET(5) = 0x00000004; // Set priority level = 1
    IPCSET(5) = 0x00000001; // Set sub-priority level = 1
// Can be done in a single operation by assigning
// IPC5SET = 0x00000005
    IFSCLR(0) = 0x00100000; // Clear the Timer5 interrupt status flag
    IECSET(0) = 0x00100000; // Enable Timer5 interrupts
    T4CONSET = 0x8000; // Start the timer



  /**  T3CON = 0x0;            // stop timer
    TMR3 = 0;               // clear timer
    PR3 = period;           // set period
    IPCSET(3) = priority;   // set priority

    IFSCLR(0) = 0x1000;      // reset timer interrupt status flag

    // enable timer interrupts
    if (interrupts == 1) { IECSET(0) = 0x1000; }

    // set prescaling
    T3CONSET = prescaling << 4;

    // enable
    T3CONSET = 0x8000;
    */
}

void disable_tmr45(void){
    __asm__ volatile("di");
    T4CON = 0x0; // Stop any 16/32-bit Timer4 operation
    T5CON = 0x0; // Stop any 16-bit Timer5 operation
    IFSCLR(0) = 0x00100000; // Clear the Timer5 interrupt status flag
    IECCLR(0) = 0x00100000; // Enable Timer5 interrupts
    __asm__ volatile("ei");
}