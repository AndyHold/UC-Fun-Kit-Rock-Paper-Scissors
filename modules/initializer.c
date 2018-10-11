#include "initializer.h"


void initialize_all (uint16_t pacer_rate, uint16_t message_rate)
{
    system_init ();
    /* Initialize tiny_gl */
    tinygl_init (pacer_rate);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (message_rate);
    navswitch_init();
    /* Initialise the bitmap displayer */
    bitmap_display_init();
    /* Initialise Pacer */
    pacer_init (pacer_rate);
    /* Initialize Infa Red */
    ir_uart_init ();
}


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
