#include "displayer.h"

/* Module to display a scrolling text until navswitch pressed. */


void display_instructions(char instructions[])
{
    /* Display Instructions for paper scissors rock */
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(instructions);
    displaytext_run();
}

void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


int displaytext_run( void )
{
    uint8_t finished = 0;
    while(!finished)
    {
        /* wait for the pacer to finish */
        pacer_wait();
        navswitch_update();
        /* TODO: Call the tinygl update function. */
        tinygl_update();
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            finished = 1;
        }

    }
    return 0;
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



static void display_column (uint8_t row_pattern, uint8_t current_column)
{

    pio_output_high(cols[previous_col]);
    uint8_t current_row = 0;

    for (current_row; current_row < 7; current_row += 1) {
        if ((row_pattern >> current_row) & 1)
        {
            pio_output_low(rows[current_row]);
        }
        else
        {
            pio_output_high(rows[current_row]);
        }
    }

    pio_output_low(cols[current_column]);
    previous_col = current_column;
}


void bitmap_display_run (const uint8_t bitmap[5])
{

    uint8_t current_column = 0;
    uint8_t finished = 0;
    while (!finished)
    {
        pacer_wait ();
        display_column (bitmap[current_column], current_column);
        current_column++;

        if (current_column > 4)
        {
            current_column = 0;
        }
    }
}



