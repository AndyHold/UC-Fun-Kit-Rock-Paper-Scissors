/** @file   displayer.c
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Displayer Module
    @note   This is the displayer module containing methods to
    display text and images on the led matrix of the fun kit.
*/


#include "displayer.h"

/* Display module for paper-scissors-rock / battleships game */



void display_instructions_init (char instructions[])
{
    /* Display Instructions for paper scissors rock */
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(instructions);
}

void display_character (char character)
{
    char buffer[3];
    buffer[0] = character;
    buffer[1] = ' ';
    buffer[2] = '\0';
    tinygl_text (buffer);
}


/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};


static uint8_t previous_col = 0;


void display_column (uint8_t row_pattern, uint8_t current_column)
{
    pio_output_high(cols[previous_col]);
    uint8_t current_row = 0;
    while (current_row < 7)
    {
        if ((row_pattern >> current_row) & 1)
        {
            pio_output_low(rows[current_row]);
        }
        else
        {
            pio_output_high(rows[current_row]);
        }
        current_row++;
    }

    pio_output_low(cols[current_column]);
    previous_col = current_column;
}



