
/* led.c */

#include <pic32mx.h>
#include "../includes/peripherals.h"


/* LEDs are mapped to PORTE
 */
void led_init(void) {
    /* Set all bits in PORTE to OUTPUT */
    TRISECLR = 0xff;
    /* Initialize all LEDs to be OFF */
    PORTECLR = 0xff;
}

void set_led(int led, int state) {
    switch (state) {
        case 0:
            TRISESET = 1 << (led - 1);
            PORTECLR = 1 << (led - 1);
            break;
        case 1:
            TRISECLR = 1 << (led - 1);
            PORTESET = 1 << (led - 1);
            break;
        case 2:
            if (PORTE & 1 << (led - 1))
                TRISESET = 1 << (led - 1);
            else
                TRISECLR = 1 << (led - 1);
            PORTEINV = 1 << (led - 1);
            break;
    }
}

void led_clear(void) {
    PORTECLR = 0xff;
}


void led_set(uint8_t code) {
    PORTECLR = ~code;
    PORTESET = code;
}

