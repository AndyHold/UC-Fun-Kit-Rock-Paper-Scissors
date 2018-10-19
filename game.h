/** @file   game.h
    @author A.Holden & R.Yoshinari
    @date   17 Oct 2018
    @brief  Paper Scissors Rock Game Made for the UCFK4 (UC Fun Kit 4)
    @note   This is the main game module containing the main method.
*/


#ifndef GAME_H
#define GAME_H


/** All included modules required by game.c */
#include "system.h"
#include "button.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "modules/displayer.h"
#include "modules/sound.h"
#include "led.h"
#include "task.h"
#include "mmelody.h"
#include "string.h"


/** define BTN on Port D pin 7 */
#define BTN_PIO PIO_DEFINE (PORT_D, 7)


/** Initialize messages for display */
static char intro_message[11] = "JANKEN PON";
static char instruction[14] = "CHOOSE OPTION";
static char win[4] = "WIN";
static char lose[5] = "LOSE";
static char draw[5] = "DRAW";


/** Initialize sound strings */
static char win_sound[] = "A,A#,B,C+/";
static char lose_sound[] = "A#AG#G/";
static char error_sound[] = "C,C,C,";
static char select_sound[] = "B5,E6";
static char move_sound[] = "E6,G";
static char intro_music[] =
{
#include "Axel_F.mmel"
"    >"
};
static char waiting_music[] =
{
#include "zelda_shop.mmel"
"    >"
};


/** Define an enum of the state the game is in at each point to be
 *  used in switch statements during the tasks. */
typedef enum {STATE_INITIAL_INSTRUCTIONS, STATE_CHOOSE_OPTION,
              STATE_WAITING_FOR_OPPONENT, STATE_SHOW_RESULT,
              STATE_INTRODUCTION} state_t;


/** Set the initial state of the game when booted. */
static state_t state = STATE_INTRODUCTION;


/** Initialize booleans and selection variables */
static char your_selection = 0;
static char opponents_selection = 0;
static uint8_t option = 0;
static uint8_t current_column = 0;
static uint8_t counter = 0;
static uint8_t winner = 0;
static uint8_t led_state = 0;


/** Options for paper scissors rock*/
const char options[3] = { 'P', 'S', 'R' };


/** Bitmap for face to display while waiting for opponent */
const uint8_t waiting_bitmap[5] =
{
    0x32, 0x62, 0x6C, 0x62, 0x32
};


#endif
