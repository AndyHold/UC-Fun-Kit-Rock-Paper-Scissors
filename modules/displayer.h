/** @file   displayer.h
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Displayer Module
    @note   This is the displayer module containing methods to
    display text and images on the led matrix of the fun kit.
*/


#ifndef DISPLAYER_H
#define DISPLAYER_H


/** All included modules required by displayer.c */
#include "system.h"
#include "pio.h"
#include "tinygl.h"
#include "../fonts/font3x5_1.h"


/** Initialise task rates for display */
enum {DISPLAY_TASK_RATE = 1000};
enum {MESSAGE_RATE = 20};


/** Display scrolling instructions */
void display_instructions_init (char instructions[]);


/** Initialize common display elements for text */
void text_display_init ( uint16_t pacer_rate, uint16_t message_rate );


/** Initialize Bitmap Display */
void bitmap_display_init ( void );


/** Display scrolling a single character */
void display_character (char character);


/** Run display of text */
int displaytext_run ( void );


/** Display a column */
void display_column (uint8_t row_pattern, uint8_t current_column);
#endif
