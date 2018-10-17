#include "game.h"


/** Define polling rates in Hz.  The sound task needs to be the highest
 *  priority since any jitter will make the sound awful.  */
enum {BUTTON_TASK_RATE = 20};


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


/** Navswitch Task to be performed. */
void button_task ( __unused__ void *data )
{
    navswitch_update();
    switch (state)
    {
    case STATE_INTRODUCTION:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            display_instructions_init (instruction);
            mmelody_play (melody, select_sound);
            state = STATE_INITIAL_INSTRUCTIONS;
        }
        break;

    case STATE_INITIAL_INSTRUCTIONS:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            mmelody_play (melody, select_sound);
            state = STATE_CHOOSE_OPTION;
        }
        break;

    case STATE_CHOOSE_OPTION:
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
        /* If navswitch north or east change to another option */
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
        /* If navswitch pressed transmit your option to the opponent */
        if (!your_selection)
        {
            if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
            {
                mmelody_play (melody, select_sound);
                ir_uart_putc( options[option] );
                your_selection = options[option];
                if (!opponents_selection)
                {
                    tinygl_clear ();
                    bitmap_display_init ();
                    mmelody_play (melody, waiting_music);
                    state = STATE_WAITING_FOR_OPPONENT;
                }
                else
                {
                    // Change your turn
                    compare_move(your_selection, opponents_selection);
                    state = STATE_SHOW_WINNER;
                }
            }
        }
        /* If infra red has recieved an option save it*/
        if (!opponents_selection)
        {
            if ( ir_uart_read_ready_p() )
            {
                char recieved_char = ir_uart_getc();
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
            if ( ir_uart_read_ready_p() )
            {
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
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            mmelody_play (melody, select_sound);
            if (opponents_selection == your_selection)
            {
                reset_game ();
                display_instructions_init (instruction);
                state = STATE_INTRODUCTION;
            }
            else
            {
                reset_game ();
                display_instructions_init (intro_message);
                state = STATE_INITIAL_INSTRUCTIONS;
            }
        }
        break;
    }
}


/** Main method (Where program thread starts. */
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
         .data = 0}
    };

    /* Initialise Everything  */
    initialize_all(DISPLAY_TASK_RATE, MESSAGE_RATE);
    display_instructions_init (intro_message);
    tweeter_task_init ();
    sound_task_init ();

    /* Play introduction song */
    mmelody_play(melody, intro_music);

    /* Start task scheduler */
    task_schedule (tasks, ARRAY_SIZE (tasks));
}
