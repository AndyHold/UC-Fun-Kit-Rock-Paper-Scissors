/** @file   game.c
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Paper Scissors Rock Game Made for the UCFK4 (UC Fun Kit 4)
    @note   This is the main game module containing the main method.
*/


/** Include game.h file which contains all other imported modules. */
#include "game.h"


/** Define polling rates in Hz.  The sound task needs to be the highest
 *  priority since any jitter will make the sound awful.  */
enum {BUTTON_TASK_RATE = 20};
enum {FLASHER_TASK_RATE = 10};


/** Initialise button states. */
int previous_state = 1;
int current_state = 0;


/** Reset the games values to defaults so game can be played again
 *  from the start. */
void reset_game ( void )
{
    your_selection = 0;
    opponents_selection = 0;
    option = 0;
    winner = 0;
}


/** Method to compute the winner of the game and set the appropriate
 *  message and sound effect for the next state. */
static void compare_move(char your_selection, char opponents_selection)
{
    /* Re-initialize scrolling text */
    text_display_init (DISPLAY_TASK_RATE, MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    /*  Checks for a draw  */
    if (your_selection == opponents_selection)
    {
        /* Initialize draw scrolling text and error sound */
        display_instructions_init (draw);
        mmelody_play (melody, error_sound);
    }

    /*  Rock state */
    if (your_selection == 'R')
    {
        /* If opponent selects Scissors */
        if (opponents_selection == 'S')
        {
            /* Set winner to true, initialize win scrolling text and win sound */
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        /* If opponent selects paper */
        else if (opponents_selection == 'P')
        {
            /* Leave winner on false, initialize lose scrolling text and lose sound */
            display_instructions_init (lose);
            mmelody_play (melody, lose_sound);
        }
    }

    /*  Paper state */
    if (your_selection == 'P')
    {
        /* If opponent selects Rock */
        if (opponents_selection == 'R')
        {
            /* Set winner to true, initialize win scrolling text and win sound */
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        /* If opponent selects Scissors */
        else if (opponents_selection == 'S')
        {
            /* Leave winner on false, initialize lose scrolling text and lose sound */
            display_instructions_init (lose);
            mmelody_play (melody, lose_sound);
        }
    }

    /*  Scissors state  */
    if (your_selection == 'S')
    {
        /* If opponent selects paper */
        if (opponents_selection == 'P')
        {
            /* Set winner to true, initialize win scrolling text and win sound */
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        /* If opponent selects Rock */
        else if (opponents_selection == 'R')
        {
            /* Leave winner on false, initialize lose scrolling text and lose sound */
            display_instructions_init (lose);
            mmelody_play (melody, lose_sound);
        }
    }
}


/** Display task to be performed. */
void display_task ( __unused__ void *data )
{
    /* Switch statement for updating the tinygl in each case */
    switch (state)
    {
    case STATE_CHOOSE_OPTION:
        /* Display a single character from the options list */
        tinygl_update ();
        display_character ( options[option] );
        break;

    case STATE_WAITING_FOR_OPPONENT:
        /* Display the waiting bitmap */
        display_column (waiting_bitmap[current_column], current_column);
        current_column++;
        /* If you have reached the end of the options list, wrap around to
         * the start again. */
        if (current_column == 5)
            current_column = 0;
        break;

    /* All other states do the same thing: */
    default:
        tinygl_update ();
    }
}

/** Flasher Task to be performed. */
void flasher_task ( __unused__ void *data )
{
    /* Checks if the state is show winner */
    if (state == STATE_SHOW_RESULT) {
        /* Enables the led to flash */
        if (winner) {
            led_set  (LED1, led_state);
            led_state = !led_state;
        }
        /* Enables the led to flash at half the frequency */
        else if (counter % 2)
        {
            led_set  (LED1, led_state);
            led_state = !led_state;
        }
        /* Increment the counter */
        counter++;
    }
}

/** Navswitch Task to be performed. */
void button_task ( __unused__ void *data )
{
    /* Update the state of each of the buttons */
    navswitch_update();
    button_update ();

    /* Switch statement for describing navswitch actions for each case */
    switch (state)
    {
    case STATE_INTRODUCTION:
        /* If navswitch push change to the instruction state */
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            /* Set the pre-game instruction to display in scroll mode.
             * Set the speed and melody for the select sound.
             * Change state to theinitial instruction state. */
            display_instructions_init (instruction);
            mmelody_speed_set (melody, TUNE_BPM_RATE);
            mmelody_play (melody, select_sound);
            state = STATE_INITIAL_INSTRUCTIONS;
        }
        break;

    case STATE_INITIAL_INSTRUCTIONS:
        /* If navswtich push change to the choose option state */
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) || button_push_event_p ( 0 ) )
        {
            /* Set the text mode to step so it only displays single characters.
             * Set the melody to select sound.
             * Change the state to Choose Option State. */
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            mmelody_play (melody, select_sound);
            state = STATE_CHOOSE_OPTION;
        }
        break;

    case STATE_CHOOSE_OPTION:
        /* If navswitch north or west change to another option */
        if ( navswitch_push_event_p ( NAVSWITCH_NORTH ) || navswitch_push_event_p ( NAVSWITCH_WEST ) )
        {
            /* Set the melody to select sound. */
            mmelody_play (melody, move_sound);
            /* If the option has reached the end of the option list,
             * wrap around to the beginning, otherwise increment */
            if (option == 2)
            {
                option = 0;
            }
            else
            {
                option++;
            }
        }
        /* If navswitch south or east change to another option */
        if ( navswitch_push_event_p ( NAVSWITCH_SOUTH ) || navswitch_push_event_p ( NAVSWITCH_EAST ) )
        {
            /* Set the melody to select sound. */
            mmelody_play (melody, move_sound);
            /* If the option has reached the beggining of the option list,
             * wrap around to the end, otherwise increment */
            if (option == 0)
            {
                option = 2;
            }
            else
            {
                option--;
            }
        }
        /* If navswitch push transmit your option to the opponent */
        if (!your_selection)
        {
            if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) || button_push_event_p ( 0 ) )
            {
                /* Set the melody to select sound. */
                mmelody_play (melody, select_sound);
                /* Send your chosen option to the oponent */
                ir_uart_putc( options[option] );
                /* Set your selection */
                your_selection = options[option];
                /* If opponent selection has not been recieved */
                if (!opponents_selection)
                {
                    /* Clear the text initialisation and initialize the waiting bitmap */
                    tinygl_clear ();
                    bitmap_display_init ();
                    /* Set the melody to play the waiting music */
                    mmelody_play (melody, waiting_music);
                    /* Change state to Waiting for opponent state. */
                    state = STATE_WAITING_FOR_OPPONENT;
                }
                else
                {
                    /* Computes your selection and opponents selection */
                    compare_move(your_selection, opponents_selection);
                    /* Set the state to show result */
                    state = STATE_SHOW_RESULT;
                }
            }
        }
        /* If infra red has recieved an option save it*/
        if (!opponents_selection)
        {
            /* If RPS symbol has been recieved */
            if ( ir_uart_read_ready_p() )
            {
                /* Recieve the opponents char */
                char recieved_char = ir_uart_getc();
                /* Checks the received char is a RPS symbol */
                if (memchr(options, recieved_char, sizeof(options)))
                {
                    /* Set the opponents selection */
                    opponents_selection = recieved_char;
                }
            }
        }
        break;

    case STATE_WAITING_FOR_OPPONENT:
        /* If infra red has recieved an option save it*/
        if (!opponents_selection)
        {
            /* If RPS symbol has been recieved */
            if ( ir_uart_read_ready_p() )
            {
                /* Recieve the opponents char */
                char recieved_char = ir_uart_getc();
                /* Checks the received char is a RPS symbol */
                if (memchr(options, recieved_char, sizeof(options)))
                {
                    /* Set the opponents selection */
                    opponents_selection = recieved_char;
                    /* Check for winner and initialize next state based on result. */
                    compare_move(your_selection, opponents_selection);
                    /* Set state to Show winner */
                    state = STATE_SHOW_RESULT;
                }
            }
        }
        break;

    case STATE_SHOW_RESULT:
        /* If navswitch push move to either initial or intro state */
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) || button_push_event_p ( 0 ) )
        {
            /* Set the melody to select sound. */
            mmelody_play (melody, select_sound);
            /* Set the led to off state */
            led_set  (LED1, 0);
            /* If draw, returns to the introduction state */
            if (opponents_selection == your_selection)
            {
                /* Reset game to original state and initialize the introduction state */
                reset_game ();
                display_instructions_init (instruction);
                state = STATE_INTRODUCTION;
            }
            else
            /* If win or lose, returns to the initial state (resets) */
            {
                /* Reset game to original state and initialize the initial instructions state */
                reset_game ();
                display_instructions_init (intro_message);
                state = STATE_INITIAL_INSTRUCTIONS;
            }
        }
        break;
    }
}


/** Main method (Where program thread starts) */
int main (void)
{
    /* Initialization of the task scheduler */
    task_t tasks[] =
    {
        {.func = tweeter_task, .period = TASK_RATE / TWEETER_TASK_RATE,
         .data = 0},
        {.func = sound_task, .period = TASK_RATE / SOUND_TASK_RATE,
         .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE,
         .data = 0},
        {.func = button_task, .period = TASK_RATE / BUTTON_TASK_RATE,
         .data = 0},
        {.func = flasher_task, .period = TASK_RATE / FLASHER_TASK_RATE,
         .data = 0}
    };

    /* Initialise Everything  */
    system_init ();
    /* Initialize buttons */
    button_init ();
    navswitch_init();
    pio_config_set(BTN_PIO, PIO_INPUT);
    /* Initialize Infa Red */
    ir_uart_init ();
    /* Initialize display elements */
    text_display_init (DISPLAY_TASK_RATE, MESSAGE_RATE);
    display_instructions_init (intro_message);
    /* Initialize sound elements */
    tweeter_task_init ();
    sound_task_init ();
    /* Initialize led */
    led_init ();
    led_set  (LED1, 0);

    /* Play introduction song */
    mmelody_play(melody, intro_music);

    /* Start task scheduler */
    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 1;
}
