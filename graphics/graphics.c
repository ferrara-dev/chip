
/* game.c */

#include <stdint.h>
#include <pic32mx.h>
#include "../includes/peripherals.h"
#include "../includes/functions.h"
#include "../includes/icon.h"
#include <stdint.h>
#include "../includes/entity.h"
#include "../includes/objects.h"

uint8_t game[128 * 4] = {0};
volatile int invert = 0;
Ball ball;
Player o;
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

/** Rita en boll på skärmen **/
/*
void draw(Ball ball) {
    int i = 0;
    int j = 0;
    for (i = 0; i < 12; i++) {
        graphics_set(ball.ball[i][0], ball.ball[i][1], 1);
    }

}

*/
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

void graphics_print(int offset, int line, char const *chrv) {
    if (offset < 0 || offset >= DSP_COLUMNS)
        return;
    if (line < 0 || line >= DSP_PAGES)
        return;
    int i = 0;
    while (*chrv) {
        int idx;

        if (*chrv == '.')
            idx = 1;
        else if (*chrv == ':')
            idx = 12;
        else if (*chrv >= '0' && *chrv <= '9')
            idx = *chrv - ('0' - 2);
        else if (*chrv >= 'A' && *chrv <= 'Z')
            idx = *chrv - ('A' - 13);
        else
            idx = 0;

        for (i = 0; i < FONT_WIDTH && offset < DSP_COLUMNS; ++i) {
            /* Letters are given with the bits in reverse order */
            video_buffer[offset++][line] |= reverse_byte(font[idx][i]);
        }
        ++chrv;
    }
}


/* Switch the screen between COMMAND/DATA mode
 */
static void set_command_mode(void) {
    PORTFCLR = 0x10;
}

static void set_data_mode(void) {
    PORTFSET = 0x10;
}
