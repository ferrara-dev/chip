
/* button.c */

#include <string.h>
#include "../includes/label.h"
#include "../includes/icon.h"
#include "../includes/graphics.h"

void label_cons(Label *label, char const *string, void (*function)(void))
{
	if(strlen(string) < LABEL_BUFFER_SIZE)
		strcpy(label->string, string);
	else
		strcpy(label->string, "INVALID");
	label->function = function;
}

void label_press(Label const *label)
{
	if(label->function != NULL)
		label->function();
}


void label_draw(Label const *label, int line)
{
	int const sz     = strlen(label->string);
	int const offset = (DSP_COLUMNS / 2) - ((sz / 2) * FONT_WIDTH_8) - ((sz & 1) ? (FONT_WIDTH_8 / 2) : 0);
	
	graphics_print(offset, line, label->string);
}