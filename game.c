#include "system.h"
#include "button.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "modules/displayer.h"
#include "modules/initializer.h"
#include "task.h"
#include "tweeter.h"
#include "mmelody.h"


#define PACER_RATE 500
#define MESSAGE_RATE 20
#define PIEZO_PIO PIO_DEFINE (PORT_D, 6)


static const char options[3] =
{
    'P', 'S', 'R'
};


/** Define polling rates in Hz.  The sound task needs to be the highest
    priority since any jitter will make the sound awful.  */
enum {TWEETER_TASK_RATE = 20000};
enum {SOUND_TASK_RATE = 100};
enum {DISPLAY_TASK_RATE = 300};
enum {BUTTON_TASK_RATE = 20};
enum {TUNE_BPM_RATE = 180};


typedef enum {STATE_INITIAL_INSTRUCTIONS, STATE_CHOOSE_OPTION,
              STATE_WAITING_FOR_OPPONENT, STATE_SHOW_WINNER,
              STATE_INTRODUCTION} state_t;
static state_t state = STATE_INTRODUCTION;


static const uint8_t waiting_bitmap[5] =
{
    0x32, 0x62, 0x6C, 0x62, 0x32
};


static const char game_intro_tune[] =
{
#include "mario_theme.mmel"
"> "
};
/*
static const char waiting_tune[] = "";
*/
static const char losing_sound[] = "A#AG#G/";
static const char winning_sound[] = "A,A#,B,C+/";
static const char error_sound[] = "C,C,C,";
static const char select_sound[] = "  ,B5,E6";
static const char move_sound[] = "E6G6E6G6";

static char your_selection = 0;
static char opponents_selection = 0;
static uint8_t option = 0;
static uint8_t current_column = 0;

static tweeter_scale_t scale_table[] = TWEETER_SCALE_TABLE (TWEETER_TASK_RATE);
static tweeter_t tweeter;
static mmelody_t melody;
static mmelody_obj_t melody_info;
static tweeter_obj_t tweeter_info;


/** Initialise button states */
int previous_state = 1;
int current_state = 0;


static void tweeter_task_init (void)
{
    tweeter = tweeter_init (&tweeter_info, TWEETER_TASK_RATE, scale_table);

    pio_config_set (PIEZO_PIO, PIO_OUTPUT_LOW);
}


static void tweeter_task (__unused__ void *data)
{
    pio_output_set (PIEZO_PIO, tweeter_update (tweeter));
}


static void sound_task_init (void)
{
    melody = mmelody_init (&melody_info, SOUND_TASK_RATE,
               (mmelody_callback_t) tweeter_note_play, tweeter);

    mmelody_speed_set (melody, TUNE_BPM_RATE);
}

static void compare_move(char your_selection, char opponents_selection)
{
    /* Re-initialize scrolling text */
    tinygl_init0 (PACER_RATE, MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);

    /*  Checks for a draw  */
    if (your_selection == opponents_selection)
    {
        display_instructions_init ("DRAW");
        mmelody_play (melody, error_sound);
    }

    /*  Rock state */
    if (your_selection == 'R')
    {
        if (opponents_selection == 'S')
        {
            display_instructions_init ("YOU WIN");
            mmelody_play (melody, winning_sound);
        }
        else if (opponents_selection == 'P')
        {
            display_instructions_init ("YOU LOSE");
            mmelody_play (melody, losing_sound);
        }
    }

    /*  Paper state */
    if (your_selection == 'P')
    {
        if (opponents_selection == 'R')
        {
            display_instructions_init ("YOU WIN");
            mmelody_play (melody, winning_sound);
        }
        else if (opponents_selection == 'S')
        {
            display_instructions_init ("YOU LOSE");
            mmelody_play (melody, losing_sound);
        }
    }

    /*  Scissors state  */
    if (your_selection == 'S')
    {
        if (opponents_selection == 'P')
        {
            display_instructions_init ("YOU WIN");
            mmelody_play (melody, winning_sound);
        }
        else if (opponents_selection == 'R')
        {
            display_instructions_init ("YOU LOSE");
            mmelody_play (melody, losing_sound);
        }
    }
}

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

void navswitch_task ( __unused__ void *data )
{
    navswitch_update();
    switch (state)
    {
    case STATE_INTRODUCTION:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            display_instructions_init ("Paper, Scissors or Rock?");
            mmelody_play(melody, select_sound);
            state = STATE_INITIAL_INSTRUCTIONS;
        }
        break;

    case STATE_INITIAL_INSTRUCTIONS:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            mmelody_play(melody, select_sound);
            state = STATE_CHOOSE_OPTION;
        }
        break;

    case STATE_CHOOSE_OPTION:
        if ( navswitch_push_event_p ( NAVSWITCH_NORTH ) || navswitch_push_event_p ( NAVSWITCH_WEST ) )
        {
            mmelody_play(melody, move_sound);
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
            mmelody_play(melody, move_sound);
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
                mmelody_play(melody, select_sound);
                ir_uart_putc( options[option] );
                your_selection = options[option];
                if (!opponents_selection)
                {
                    tinygl_clear ();
                    bitmap_display_init ();
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
                opponents_selection = ir_uart_getc();
            }
        }
        break;

    case STATE_WAITING_FOR_OPPONENT:
        /* If infra red has recieved an option save it*/
        if (!opponents_selection)
        {
            mmelody_play(melody, game_intro_tune);
            if ( ir_uart_read_ready_p() )
            {
                opponents_selection = ir_uart_getc();
                compare_move(your_selection, opponents_selection);
                state = STATE_SHOW_WINNER;
            }
        }
        break;

    case STATE_SHOW_WINNER:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            mmelody_play(melody, select_sound);
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            if (opponents_selection == your_selection)
            {
                display_instructions_init ("BATTLESHIPS BY AH AND RY");
                state = STATE_INTRODUCTION;
            }
            display_instructions_init ("Paper, Scissors or Rock?");
            state = STATE_INITIAL_INSTRUCTIONS;
        }
        break;
    }
}


void sound_task ( __unused__ void *data )
{
    mmelody_update (melody);
}


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
        {.func = navswitch_task, .period = TASK_RATE / BUTTON_TASK_RATE,
         .data = 0}
    };

    /* Initialise Everything  */
    initialize_all(PACER_RATE, MESSAGE_RATE);
    display_instructions_init ("BATTLESHIPS BY AH AND RY");
    tweeter_task_init ();
    sound_task_init ();

    /* Play introduction song */
    mmelody_play(melody, game_intro_tune);
/*
    mmelody_play(melody, move_sound);
*/

    task_schedule (tasks, ARRAY_SIZE (tasks));
}
