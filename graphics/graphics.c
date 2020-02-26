
/* game.c */

#include <stdint.h>
#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/graphics.h"
#include "../includes/functions.h"
#include "../includes/icon.h"
#include <stdlib.h>
#include "../includes/entity.h"
#include "../includes/objects.h"
#include "math.h"

#define DISPLAY_VDD PORTFbits.RF6
#define DISPLAY_VBATT PORTFbits.RF5
#define DISPLAY_COMMAND_DATA PORTFbits.RF4
#define DISPLAY_RESET PORTGbits.RG9

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

uint8_t game[128 * 4] = {0};
volatile int invert = 0;
/* Pixel manipulation will take place on this static video buffer
 * which will be written to the screen every tick
 */
static int8_t video_buffer[DSP_COLUMNS][DSP_PAGES];

static void set_command_mode(void);

static void set_data_mode(void);

void quicksleep(int);


/* Set up the screen
 */
void graphics_init(void) {
    /* BEGINNING OF COPIED CODE */
    set_command_mode();
    quicksleep(10);
    PORTFCLR = 0x40; /* Activate display VDD */
    quicksleep(1000000);

    spi_send_recv(0xAE);
    PORTGCLR = 0x200; /* Activate reset */
    quicksleep(10);
    PORTGSET = 0x200; /* Deactivate reset */
    quicksleep(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    PORTFCLR = 0x20; /* Activate VBAT */
    quicksleep(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);

    /* END OF COPIED CODE */
}

/*
 * Draw single pixel on display by
 * converting coordinates to SPI
 * compatible format.
 */
void lightUpPixel(int x, int y) {
    short offset = 0;
    if (y > 0) { offset = y / 8; }
    game[offset * 128 + x] |= 1 << (y - offset * 8);
}

void draw(Object o, int draw) {
    int i, j;
    for (i = 0; i < o.size; i++)
        for (j = 0; j < o.size; j++)
            if (o.objForm[i][j]) {
                if (draw)
                    graphics_set(i + o.posX, j + o.posY, 1);
                else
                    graphics_set(i + o.posX, j + o.posY, 0);

            }
}


/* Set one pixel to either white or black
 */
void graphics_set(int x, int y, int val) {
    if (x >= DSP_COLUMNS || x < 0)
        return;
    if (y >= DSP_ROWS || y < 0)
        return;
    /* As the page size is 8 bits, y divided by 8 will give the correct */
    /* page number and the lower 3 bits will give the bit offset */

    /* Clear the pixel that is being written to */
    video_buffer[x][y >> 3] &= ~(1 << (y & 0x07));
    /* Write */
    video_buffer[x][y >> 3] |= (val ? 1 : 0) << (y & 0x07);
}

/* Write the private video buffer to screen
 */
void graphics_reload(void) {
    int p = 0;
    for (p = 0; p < DSP_PAGES; ++p) {
        set_command_mode();
        /* Write to display */
        spi_send_recv(0x22);
        /* Specify which page we're writing to */
        spi_send_recv(p);
        /* Set byte ordering to start at left side of screen*/
        spi_send_recv(0x00);
        spi_send_recv(0x10);

        set_data_mode();

        /* Write the data to display */
        for (int x = 0; x < DSP_COLUMNS; ++x) {
            spi_send_recv(video_buffer[x][p]);
        }
    }
}


/* If the invert flag is set, invert all the
 * colors on the screen
 */
void graphics_postprocess(void) {
    if (invert) {
        int p = 0;
        int x = 0;
        for (p = 0; p < DSP_PAGES; ++p) {
            for (x = 0; x < DSP_COLUMNS; ++x) {
                video_buffer[x][p] = ~video_buffer[x][p];
            }
        }
    }
}


/* Clear (black) the video buffer
 */
void graphics_clear(void) {
    int p = 0;
    int x = 0;
    for (p = 0; p < DSP_PAGES; ++p) {
        for (x = 0; x < DSP_COLUMNS; ++x) {
            video_buffer[x][p] = 0x00;
        }
    }
}


/* Print a string to the screen, lines can be between 0 and DSP_PAGES - 1
 * and offset is limited by DSP_COLUMNS 
 * Only supports capital letters, numbers, <:> and <.>
 */
static int ascii_table_initialized = 0;
char ascii[97];
void reverse_font(int8_t *f);
void init_ascii(void) {
  //  for(int j = 0; j < 128; j++){
   //     font[j] = reverse_font(font8x8[j]);
   // }

    int a = 0;
    char k = ' ';
    while (k < '~') {
        ascii[a] = k;
        k++;
        a++;
    }
    ascii_table_initialized = 1;

}

void graphics_print(int offset, int line, char const *chrv) {
    if (offset < 0 || offset >= DSP_COLUMNS)
        return;
    if (line < 0 || line >= DSP_PAGES)
        return;
    if (!ascii_table_initialized)
        init_ascii();

    int i = 0;
    int temp;
    int j = 0;

    while (*chrv) {
        int idx;
        while (j < 97) {
            if (*chrv == ascii[j]) {
                idx = j;
                break;
            }
            j++;
        }

        for (i = 0; i < FONT_WIDTH && offset < DSP_COLUMNS; ++i) {
            /* Letters are given with the bits in reverse order */
            video_buffer[offset++][line] |= reverse_byte(font[idx][i]);
        }
        ++chrv;
        j = 0;
    }
}

/****** draw_borders(void) ******
 * Draws borders on the display
 * no return value
 ********************************/
void draw_borders(void) {
    int i;
    for (i = 0; i < DSP_COLUMNS; i++) {
        graphics_set(i, DSP_ROWS - 1, 1);
        graphics_set(i, 0, 1);
    }
}


/**
 * Renders the full screen
 */
void renderScreen(uint8_t arr[]) {
    int i, j;

    for(i = 0; i < 4; i++) {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));

        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK;

        for(j = 0; j < 128; j++)
            spi_send_recv(arr[i*128 + j]);
    }
}

#define ITOA_BUFSIZ ( 24 )

char *itoaconv(int num) {
    register int i, sign;
    static char itoa_buffer[ITOA_BUFSIZ];
    static const char maxneg[] = "-2147483648";

    itoa_buffer[ITOA_BUFSIZ - 1] = 0;   /* Insert the end-of-string marker. */
    sign = num;                           /* Save sign. */
    if (num < 0 && num - 1 > 0)          /* Check for most negative integer */
    {
        for (i = 0; i < sizeof(maxneg); i += 1)
            itoa_buffer[i + 1] = maxneg[i];
        i = 0;
    } else {
        if (num < 0) num = -num;           /* Make number positive. */
        i = ITOA_BUFSIZ - 2;                /* Location for first ASCII digit. */
        do {
            itoa_buffer[i] = num % 10 + '0';/* Insert next digit. */
            num = num / 10;                   /* Remove digit from number. */
            i -= 1;                           /* Move index to next empty position. */
        } while (num > 0);
        if (sign < 0) {
            itoa_buffer[i] = '-';
            i -= 1;
        }
    }
    /* Since the loop always sets the index i to the next empty position,
     * we must add 1 in order to return a pointer to the first occupied position. */
    return (&itoa_buffer[i + 1]);
}

/* Switch the screen between COMMAND/DATA mode
 */
static void set_command_mode(void) {
    PORTFCLR = 0x10;
}

static void set_data_mode(void) {
    PORTFSET = 0x10;
}
