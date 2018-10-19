/** @file   displayer.c
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Displayer Module
    @note   This is the displayer module containing methods to
    display text and images on the led matrix of the fun kit.
*/


/** Include displayer.h file which contains all other imported modules. */
#include "displayer.h"


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


/** Initialize a variable to keep track of which column was las displayed. */
static uint8_t previous_col = 0;


/** Display scrolling instructions */
void display_instructions_init (char instructions[])
{
    /* Display Instructions for paper scissors rock */
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(instructions);
}


/** Initialize common display elements for text */
void text_display_init ( uint16_t task_rate, uint16_t message_rate )
{
    tinygl_init (task_rate);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (message_rate);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
}


/** Initialize Bitmap Display */
void bitmap_display_init ( void )
{
    /* Initialise LED matrix pins.  */
    pio_config_set (LEDMAT_COL1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_COL5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW1_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW2_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW3_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW4_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW5_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW6_PIO, PIO_OUTPUT_HIGH);
    pio_config_set (LEDMAT_ROW7_PIO, PIO_OUTPUT_HIGH);
}


/** Display a single character */
void display_character (char character)
{
    /* Create a buffer of size 3 with the 0th index as the current option,
     * the 1st index as a space and the 2nd index as the null byte.
     * Then initialize tinygy with this buffer string. */
    char buffer[3];
    buffer[0] = character;
    buffer[1] = ' ';
    buffer[2] = '\0';
    tinygl_text (buffer);
}


/** Display a column */
void display_column (uint8_t row_pattern, uint8_t current_column)
{
    /* Set previous column to high */
    pio_output_high(cols[previous_col]);
    /* Reset current row to 0 */
    uint8_t current_row = 0;
    /* Iterate over the bits in the bitmap for current row and set them as required. */
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
        /* Increment current row */
        current_row++;
    }
    /* Set current columns input to low to display the column. */
    pio_output_low(cols[current_column]);
    /* Set previous column to this column as we are finished. */
    previous_col = current_column;
}
