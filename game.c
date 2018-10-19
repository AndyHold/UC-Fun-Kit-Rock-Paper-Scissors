/** @file   game.c
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Paper Scissors Rock Game
    @note   This is the main game module containing the main method.
*/


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
        display_instructions_init (draw);
        mmelody_play (melody, error_sound);
    }

    /*  Rock state */
    if (your_selection == 'R')
    {
        if (opponents_selection == 'S')
        {
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        else if (opponents_selection == 'P')
        {
            display_instructions_init (lose);
            mmelody_play (melody, lose_sound);
        }
    }

    /*  Paper state */
    if (your_selection == 'P')
    {
        if (opponents_selection == 'R')
        {
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        else if (opponents_selection == 'S')
        {
            display_instructions_init (lose);
            mmelody_play (melody, lose_sound);
        }
    }

    /*  Scissors state  */
    if (your_selection == 'S')
    {
        if (opponents_selection == 'P')
        {
            winner = 1;
            display_instructions_init (win);
            mmelody_play (melody, win_sound);
        }
        else if (opponents_selection == 'R')
        {
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
    case STATE_INTRODUCTION:
        tinygl_update ();
        break;

    case STATE_INITIAL_INSTRUCTIONS:
        tinygl_update ();
        break;

    case STATE_CHOOSE_OPTION:
        tinygl_update ();
        display_character ( options[option] );
        break;

    case STATE_WAITING_FOR_OPPONENT:
        display_column (waiting_bitmap[current_column], current_column);
        current_column++;
        if (current_column == 5)
            current_column = 0;
        break;

    case STATE_SHOW_WINNER:
        tinygl_update ();
        break;
    }
}

/** Flasher Task to be performed. */
void flasher_task ( __unused__ void *data )
{
    /* Checks if the state is show winner */
    if (state == STATE_SHOW_WINNER) {
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
        counter++;
    }
}

/** Navswitch Task to be performed. */
void button_task ( __unused__ void *data )
{
    navswitch_update();
    button_update ();

    /* Switch statement for describing navswitch actions for each case */
    switch (state)
    {
    case STATE_INTRODUCTION:
        /* If navswitch push change to the instruction state */
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
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
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            mmelody_play (melody, select_sound);
            state = STATE_CHOOSE_OPTION;
        }
        break;

    case STATE_CHOOSE_OPTION:
        /* If navswitch north or west change to another option */
        if ( navswitch_push_event_p ( NAVSWITCH_NORTH ) || navswitch_push_event_p ( NAVSWITCH_WEST ) )
        {
            mmelody_play (melody, move_sound);
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
            mmelody_play (melody, move_sound);
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
                mmelody_play (melody, select_sound);
                ir_uart_putc( options[option] );
                your_selection = options[option];
                /* If opponent selection has not been recieved */
                if (!opponents_selection)
                {
                    tinygl_clear ();
                    bitmap_display_init ();
                    mmelody_play (melody, waiting_music);
                    state = STATE_WAITING_FOR_OPPONENT;
                }
                else
                {
                    /* Computes your selection and opponents selection */
                    compare_move(your_selection, opponents_selection);
                    state = STATE_SHOW_WINNER;
                }
            }
        }
        /* If infra red has recieved an option save it*/
        if (!opponents_selection)
        {
            /* If RPS symbol has been recieved */
            if ( ir_uart_read_ready_p() )
            {
                char recieved_char = ir_uart_getc();
                /* Checks the received char is a RPS symbol */
                if (memchr(options, recieved_char, sizeof(options))) {
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
                /* Checks the received char is a RPS symbol */
                char recieved_char = ir_uart_getc();
                if (memchr(options, recieved_char, sizeof(options))) {
                    opponents_selection = recieved_char;
                    compare_move(your_selection, opponents_selection);
                    state = STATE_SHOW_WINNER;
                }
            }
        }
        break;

    case STATE_SHOW_WINNER:
        /* If navswitch push move to either initial or intro state */
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) || button_push_event_p ( 0 ) )
        {
            mmelody_play (melody, select_sound);
            led_set  (LED1, 0);
            /* If draw, returns to the introduction state */
            if (opponents_selection == your_selection)
            {
                reset_game ();
                display_instructions_init (instruction);
                state = STATE_INTRODUCTION;
            }
            else
            /* If win or lose, returns to the initial state (resets) */
            {
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
    initialize_all(DISPLAY_TASK_RATE, MESSAGE_RATE);
    display_instructions_init (intro_message);
    tweeter_task_init ();
    sound_task_init ();
    led_init ();
    led_set  (LED1, 0);

    /* Play introduction song */
    mmelody_play(melody, intro_music);

    /* Start task scheduler */
    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 1;
}
