
/* button.h */

#pragma once
#define LABEL_BUFFER_SIZE 16 /* Highest amount of 8x8 characters that can fit on one line */

/* Labels can also be used as menu titles or information,
 * by setting the function variable to NULL
 */

typedef struct LABEL {
    void (*function)(void);

    char string[LABEL_BUFFER_SIZE];
} Label;

void label_cons(Label *, char const *, void (*)(void));

void label_press(Label const *);

void label_draw(Label const *, int);
