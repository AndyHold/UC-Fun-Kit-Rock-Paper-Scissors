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
    'P',
    'S',
    'R'
};


void display_instructions()
{
    /* Display Instructions for paper scissors rock */
    char instructions[20] = "Choose your option!\0";
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

int main (void)
{

    /* Initialise Everything  */
    initialize_all(PACER_RATE, MESSAGE_RATE);

    /* Initialize pacer to run at 500hz */
    display_instructions();
    int setupFinished = 0;
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);

    /* Start paper scissors rock loop */
    while(!setupFinished)
    {
        int selectionMade = 0;
        int opponentsSelection = 0;
        int option = 0;

        while(!selectionMade || !opponentsSelection)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update();
            if ( navswitch_push_event_p ( NAVSWITCH_NORTH ) )
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

            if ( navswitch_push_event_p ( NAVSWITCH_SOUTH ) )
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

            /* TODO: Transmit the character over IR on a NAVSWITCH_PUSH
            event.  */
            if (!selectionMade)
            {
                if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
                {
                    ir_uart_putc( options[option] );
                    selectionMade = 1;
                }
            }

            if (!opponentsSelection)
            {
                if ( ir_uart_read_ready_p() )
                {
                    opponentsSelection = ir_uart_getc();
                }
            }
            display_character ( options[option] );
        }
    }
}
