#ifndef DISPLAYER_H
#define DISPLAYER_H

#include "system.h"
#include "pio.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "string.h"
#include "navswitch.h"


enum {DISPLAY_TASK_RATE = 300};
enum {MESSAGE_RATE = 20};


/** Display scrolling instructions */
void display_instructions_init (char instructions[]);


/** Display scrolling a single character */
void display_character (char character);


/** Run display of text */
int displaytext_run ( void );


/** Display a column */
void display_column (uint8_t row_pattern, uint8_t current_column);
#endif
