
#pragma once

void graphics_init(void);
void draw(Object, int);

void graphics_set(int, int, int);
void graphics_reload(void);
void graphics_postprocess(void);
void graphics_clear(void);
void draw_borders(void);
void graphics_print(int offset, int line, char const *chrv);

void display_string(int line, char *s);
void display_image(int x, const uint8_t *data);
char * itoaconv( int  );
void display_update(void);
/* Declare bitmap array containing font */
extern const uint8_t const fonts[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

void draw_Circle(void);