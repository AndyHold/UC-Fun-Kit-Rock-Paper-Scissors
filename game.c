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


#define PACER_RATE 500
#define MESSAGE_RATE 20


static const char options[3] =
{
    'P', 'S', 'R'
};


/** Define polling rates in Hz.  The sound task needs to be the highest
    priority since any jitter will make the sound awful.  */
enum {SOUND_TASK_RATE = 5000};
enum {DISPLAY_TASK_RATE = 300};
enum {BUTTON_TASK_RATE = 20};


typedef enum {STATE_INITIAL_INSTRUCTIONS, STATE_CHOOSE_OPTION,
              STATE_WAITING_FOR_OPPONENT, STATE_SHOW_WINNER,
              STATE_DISPLAY_TURN, STATE_DISPLAY_DEPLOY_INSTRUCTIONS,
              STATE_DEPLOY, STATE_ATTACK_INSTRUCTIONS,
              STATE_CHOOSE_ATTACK, STATE_REJECT_ATTACK,
              STATE_ACCEPT_ATTACK, STATE_HIT_ATTACK, STATE_SINK_ATTACK,
              STATE_MISS_ATTACK, STATE_RESULT, STATE_DEFENDING_WAIT,
              STATE_ATTACK_RECEIVED, STATE_HIT_DEFENCE,
              STATE_SINK_DEFENCE, STATE_MISS_DEFENCE} state_t;

static state_t state = STATE_INITIAL_INSTRUCTIONS;


static const uint8_t waiting_bitmap[5] =
{
    0x32, 0x62, 0x6C, 0x62, 0x32
};


static const uint8_t attack_bitmap[5] =
{
    0x00, 0x00, 0x00, 0x00, 0x00
};


static const uint8_t defence_bitmap_solid[5] =
{
    0x00, 0x00, 0x00, 0x00, 0x00
};


static const uint8_t defence_bitmap_half[5] =
{
    0x00, 0x00, 0x00, 0x00, 0x00
};


static char your_selection = 0;
static char opponents_selection = 0;
static uint8_t option = 0;
static uint8_t current_column = 0;
static uint8_t your_turn = 0;
static uint8_t deployed = 0;
static uint8_t result_found = 0;


/** Initialise button states */
int previous_state = 1;
int current_state = 0;

static void compare_move(char your_move, char opponent_move)
{

    /*  Checks for a draw  */
    if (your_move == opponent_move) {
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        display_instructions_init ("DRAW");
    }
    else {
        result_found = 1;
    }

    /*  Rock state */
    else if (your_move == 'R') {
        if (oppenent_move == 'S') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU WIN");
        }
        else if (opponent_move == 'P') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU LOSE");
        }
    }

    /*  Paper state */
    else if (your_move == 'P') {
        if (opponent_move == 'R') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU WIN");
        }
        else if (opponent_move == 'S') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU LOSE");
        }
    }

    /*  Scissors state  */
    else if (your_move == 'S') {
        if (opponent_move == 'P') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU WIN");
        }
        else if (opponent_move == 'R') {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("YOU LOSE");
        }
    }
}

void display_task ( __unused__ void *data )
{
    switch (state)
    {
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

        break;

    case STATE_DISPLAY_DEPLOY_INSTRUCTIONS:
        tinygl_update();
        break;

    case STATE_DEPLOY:

        break;

    case STATE_DISPLAY_TURN:
        tinygl_update ();
        break;

    case STATE_ATTACK_INSTRUCTIONS:
        tinygl_update ();
        break;

    case STATE_CHOOSE_ATTACK:

        break;

    case STATE_REJECT_ATTACK:
        tinygl_update ();
        break;

    case STATE_ACCEPT_ATTACK:

        break;

    case STATE_HIT_ATTACK:
        tinygl_update ();
        break;

    case STATE_SINK_ATTACK:
        tinygl_update ();
        break;

    case STATE_MISS_ATTACK:
        tinygl_update ();
        break;

    case STATE_RESULT:
        tinygl_update ();
        break;

    case STATE_DEFENDING_WAIT:

        break;

    case STATE_ATTACK_RECEIVED:
        tinygl_update ();
        break;

    case STATE_HIT_DEFENCE:
        tinygl_update ();
        break;

    case STATE_SINK_DEFENCE:
        tinygl_update ();
        break;

    case STATE_MISS_DEFENCE:
        tinygl_update ();
        break;

    }
}

void navswitch_task ( __unused__ void *data )
{
    navswitch_update();
    switch (state)
    {
    case STATE_INITIAL_INSTRUCTIONS:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
            state = STATE_CHOOSE_OPTION;
        }
        break;

    case STATE_CHOOSE_OPTION:
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
            if ( ir_uart_read_ready_p() )
            {
                opponents_selection = ir_uart_getc();
                /* compute the result */
                compare_move(your_selection, opponents_selection);
                state = STATE_SHOW_WINNER;
            }
        }
        break;

    case STATE_SHOW_WINNER:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            if (result_found) {
                display_instructions_init ("Place your battleships.");
                state = STATE_DISPLAY_DEPLOY_INSTRUCTIONS;
            } else {
                display_instructions_init ("Choose your option.");
                state = STATE_INITIAL_INSTRUCTIONS;
            }
        }
        break;

    case STATE_DISPLAY_DEPLOY_INSTRUCTIONS:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_clear ();
            bitmap_display_init ();
            state = STATE_DEPLOY;
        }
        break;

    case STATE_DEPLOY:
        if (button_push_event_p (0))
/*
            rotate_ship ();
*/
        break;

    case STATE_DISPLAY_TURN:
        if (your_turn)
        {
            tinygl_init0 (PACER_RATE, MESSAGE_RATE);
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("Your turn.");
        }
        else
        {
            tinygl_init0 (PACER_RATE, MESSAGE_RATE);
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            display_instructions_init ("Their turn.");
        }
        break;

    case STATE_ATTACK_INSTRUCTIONS:
        if ( navswitch_push_event_p ( NAVSWITCH_PUSH ) )
        {
            tinygl_clear ();
            bitmap_display_init ();
            state = STATE_CHOOSE_ATTACK;
        }
        break;

    case STATE_CHOOSE_ATTACK:

        break;

    case STATE_REJECT_ATTACK:

        break;

    case STATE_ACCEPT_ATTACK:

        break;

    case STATE_HIT_ATTACK:

        break;

    case STATE_SINK_ATTACK:

        break;

    case STATE_MISS_ATTACK:

        break;

    case STATE_RESULT:

        break;

    case STATE_DEFENDING_WAIT:

        break;

    case STATE_ATTACK_RECEIVED:

        break;

    case STATE_HIT_DEFENCE:

        break;

    case STATE_SINK_DEFENCE:

        break;

    case STATE_MISS_DEFENCE:

        break;

    }
}


void sound_task ( __unused__ void *data )
{
    switch (state)
    {
    case STATE_INITIAL_INSTRUCTIONS:
/*
        mmelody_play(melody, game_intro_tune);
*/
        break;

    case STATE_CHOOSE_OPTION:

        break;

    case STATE_WAITING_FOR_OPPONENT:

        break;

    case STATE_SHOW_WINNER:

        break;

    case STATE_DISPLAY_TURN:

        break;

    case STATE_DISPLAY_DEPLOY_INSTRUCTIONS:

        break;

    case STATE_DEPLOY:

        break;

    case STATE_ATTACK_INSTRUCTIONS:

        break;

    case STATE_CHOOSE_ATTACK:

        break;

    case STATE_REJECT_ATTACK:

        break;

    case STATE_ACCEPT_ATTACK:

        break;

    case STATE_HIT_ATTACK:

        break;

    case STATE_SINK_ATTACK:

        break;

    case STATE_MISS_ATTACK:

        break;

    case STATE_RESULT:

        break;

    case STATE_DEFENDING_WAIT:

        break;

    case STATE_ATTACK_RECEIVED:

        break;

    case STATE_HIT_DEFENCE:

        break;

    case STATE_SINK_DEFENCE:

        break;

    case STATE_MISS_DEFENCE:

        break;

    }
}


int main (void)
{
    task_t tasks[] =
    {
        {.func = sound_task, .period = TASK_RATE / SOUND_TASK_RATE,
         .data = 0},
        {.func = display_task, .period = TASK_RATE / DISPLAY_TASK_RATE,
         .data = 0},
        {.func = navswitch_task, .period = TASK_RATE / BUTTON_TASK_RATE,
         .data = 0}
    };

    /* Initialise Everything  */
    initialize_all(PACER_RATE, MESSAGE_RATE);
    display_instructions_init ("Choose your option.");
    task_schedule (tasks, ARRAY_SIZE (tasks));
}
