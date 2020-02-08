
#pragma once

void graphics_init(void);
void draw(Object, int);

void graphics_set(int, int, int);
void graphics_reload(void);
void graphics_postprocess(void);
void graphics_clear(void);

void graphics_print(int offset, int line, char const *chrv);