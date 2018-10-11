#include "system.h"
#include "button.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "modules/displayer.h"

#define PACER_RATE 500
#define MESSAGE_RATE 10

static const char options[3] =
{
    'P', 'S', 'R'
};


static const uint8_t waiting_bitmap[5] =
{
    0x32, 0x62, 0x6C, 0x62, 0x32
};


int main (void)
{

    /* Initialise Everything  */
    initialize_all(PACER_RATE, MESSAGE_RATE);
    int setupFinished = 0;
    char your_selection = 0;
    char opponents_selection = 0;
    int option = 0;

    /* Call instructions to be displayed */
    display_instructions("Choose your option");

    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    /* Start paper scissors rock loop */
    while(!setupFinished)
    {
        while(!your_selection || !opponents_selection)
        {
            /* Wait for the pacer to reach its target */
            pacer_wait ();
            /* Update the tinygl and the navswitch */
            tinygl_update ();
            navswitch_update();
            /* If navswitch north or west change to another option */
            if ( navswitch_push_event_p ( NAVSWITCH_NORTH ) || navswitch_push_event_p ( NAVSWITCH_WEST ) )
            {
                if (option == 2)
                {
                    option = 0;
                }
                else
                {
                    option++;
                }
            }
            /* If navswitch north or east change to another option */
            if ( navswitch_push_event_p ( NAVSWITCH_SOUTH ) || navswitch_push_event_p ( NAVSWITCH_EAST ) )
            {
                if (option == 0)
                {
                    option = 2;
                }
                else
                {
                    option--;
                }
            }

            /* If navswitch pressed transmit your option to the opponent */
            if (!your_selection)
            {
                if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
                {
                    ir_uart_putc( options[option] );
                    tinygl_clear();
                    bitmap_display_run(waiting_bitmap);
                    your_selection = options[option];
                }
            }

            /* If infra red has recieved an option save it*/
            if (!opponents_selection)
            {
                if ( ir_uart_read_ready_p() )
                {
                    opponents_selection = ir_uart_getc();
                }
            }
            display_character ( options[option] );
        }

        /* compute winner etc here. */
    }
}
