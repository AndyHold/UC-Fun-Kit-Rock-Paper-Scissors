Rock, paper, scissors game:

Rock, paper, scissors game which can be played between the two microcontroller devices through infra-red communication.

The game utilises task scheduling to schedule the indivual states to progress the game. Two controllers will be able to
choose a move each using the navswitch, the selected moves from each device will be computed and a result will be 
returned which will be displayed on the device LED matrix. The microcontroller can also connect a buzzer as the game also 
implements sound with a different melody for each state. E.g. Choosing move and waiting for opponent states will have a 
different melody.

Basic Instructions:
    Confirming / Moving to the next state - Pushing the navswitch.
    Choosing options - Navswitch directional actions North, East, South, West.
    Display - Messages and symbols will be displayed in the LED matrix horizontally.
    Sound - Buzzer is plugged in to the microcontroller to enable sound throughout the game. 

It is split in the states of:

INTRODUCTION:
    Where an introduction message is displayed on the LED matrix by scrolling text, as a general game introduction.
    In this state, pushing the navigation switch will change the state to the following state of the scheduler. An 
    introduction melody will also be played through the buzzer.

INITIAL_INSTUCTIONS:
    The choose your option message is desplayed on the LED matrix. It will ask the user to choose an option. A push
    navswitch action will confirm that you are ready to choose a move and change the state to the choose options 
    state.

CHOOSE_OPTIONS:
    In this state, the user can choose between 'P' = Paper, 'S' = Scissors, 'R' = Rock. Where the option is displayed 
    on the LED matrix with the appropriate letter symbol. The symbol can then be changed with the navswitch button 
    actions (NORTH, WEST, SOUTH, EAST). The Navswitch push event will confirm the selection. The user will then go into 
    a WAITING_FOR_OPPONENT state if the opponent has not yet selected there option. This state will have a beep sound 
    with the navswitch actions. E.g. pushing north to change a move will create a beep sound.
    
WAITING_FOR_OPPONENT: 
    This state will be active till the user has selected their option, the microcontroller is in a waiting state to 
    recieve the oppoonents symbol through infrared communitcation. Once the opponent move is selected, the opponent
    selection and the user selection will be computed for a result to see who is victorious or in the case of a draw.
    The state will then be changed to the SHOW_WINNER state. There will also be a unique melody for the waiting state.
    
SHOW_WINNER:
    The show winner state will display the result on the LED matrix which the appropriate message, "YOU WIN", "YOU LOSE",
    or in the case of a draw "DRAW". In the case of a draw the state will go back to the INTRODUCTION state to issue a 
    redraw in selection till the winner is decided. In any other case, the state will go the beginning state. There will
    be a unique melody for the lose / win result. E.g. a celebratory melody for the winner. There is also a LED flicker. 
    So a winner with have a flashing light at a faster rate in comparison to the looser. 
    
The game will reset to the introduction after a navswitch push event.
    