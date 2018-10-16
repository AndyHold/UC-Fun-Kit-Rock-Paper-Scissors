Rock, paper, scissors game:

Rock, paper, scissors game which can be played between the two microcontroller devices through infra-red communication.

The game utilises task scheduling to schedule the indivual states to progress the game. Two controllers will be able to
choose a move each using the navswitch, the selected moves from each device will be computed and a result will be 
returned which will displayed on the device LED matrix. 

It is split in the states of:

INTRODUCTION:
    Where an introduction message is displayed on the LED matrix with scrolling text, as a general game introduction.
    In this state, pushing the navigation switch will change the state to the following state of the scheduler.

INITIAL_INSTUCTIONS:
    -

CHOOSE_OPTIONS:
    In this state, the user can choose between 'P' = Paper, 'S' = Scissors, 'R' = Rock. Where the option is displayed 
    on the LED matrix with the appropriate letter symbol. The symbol can then be changed with the navswitch button 
    actions (NORTH, WEST, SOUTH, EAST). The Navswith push event will confirm the selection. The user will go into a 
    WAITING_FOR_OPPONENT state if the opponent has not yet selected there option.
    
WAITING_FOR_OPPONENT: 
    This state will be active till the user has selected their option, the microcontroller is in a waiting state to 
    recieve the oppoonents symbol through infrared communitcation. Once the opponent move is selected, the opponent
    selection and the user selection will be computed for a result to see who is victorious or weather it is a draw.
    The state will then be changed to the SHOW_WINNER state.
    
SHOW_WINNER:
    The show winner state will display the result on the LED matrix which the appropriate message, "YOU WIN", "YOU LOSE",
    or in the case of a draw "DRAW". In the case of a draw the state will go back to the INTRODUCTION state to issue a 
    redraw in selection till the winner is decided. In any other case, the state will go the beginning state. 
    

Battleships game plan:


STATE_INITIAL_INSTRUCTIONS:
    Display instructions: “Choose your option.”
    STATE_CHOOSE_OPTION:
        Next have P, S and R be selectable whilst also waiting to recieve from the opponent,

    STATE_WAITING_FOR_OPPONENT:
        display “Waiting for opponent” once chosen.

    STATE_SHOW_WINNER:
        When both have selected compute result and display Draw and repeat or Win or Lose
        along with sounds to represent each and designate player name as player one or two
        depending on the result.
    STATE_DISPLAY_TURN:
        Display "your/their turn"
    STATE_DISPLAY_DEPLOY_INSTRUCTIONS:
        First display instructions: “use the navswitch to move your ships into position, press down
        to place your ships.”
    STATE_DEPLOY:
        Next have blinking lines of lights for ship to be placed and solid lines for ships already
        placed.
    STATE_WAITING_FOR_OPPONENT: (again)
        Once all ships have been placed save this as your defence screen and display “Waiting for
        opponent” until opponent is finished.

STATE_ATTACK_INSTRUCTIONS:
    First display instructions: “Choose a place to attack using the navswitch, press down to fire.”

    STATE_CHOOSE_ATTACK:
        On first turn: start with a blank board and a blinking single light to represent the attack co-
        ordinate. Keep a record of the board so it can be recalled when the next attack turn arrives.

        On further turns: start with the current attack board and a single blinking light to represent
        the attack co-ordinate.

        Check if co-ordinate has been used already, and reject if so.

    STATE_REJECT_ATTACK:
        If rejected, display “You have already attacked there” and make error sound, return to
        STATE_CHOOSE_ATTACK.

    STATE_ACCEPT_ATTACK:
        Else transmit the co-ordinate to the opponent and await response.

    STATE_HIT_ATTACK:
        If hit-not sink display “Hit!!!” and make hit sound, this point will now be a solid light
        on the attack screen with full brightness.
    STATE_SINK_ATTACK:
        If hit-sink display “Ship Sunk!!!” and make sink sound, the entire ship that has been
        sunk will now blink with full brightness for 3 seconds then remain solid with full
        brightness, check if won.
    STATE_MISS_ATTACK:
        If Miss display “miss...” and make miss sound, this point will now be a solid light on
        the attack screen with half brightness.

    STATE_RESULT:
        If all ships sunk display “You win!!!” and play win sound.

STATE_DEFENDING_WAIT:
    Display the defence screen in its current state and wait for an attack to be recieved.

    STATE_ATTACK_RECEIVED:
        Check if attack is a hit, a hit-sink, or a miss and transmit the result. Also display “Hit...”,
        “Miss!!!”, or “Sink...” along with correct sound.

    STATE_HIT_DEFENCE:
        If Hit-not sink: this point will not be a blinking solid with full brightness
    STATE_SINK_DEFENCE:
        If Sink: the whole ship will blink for 3 seconds then become solid half brightness.
    STATE_MISS_DEFENCE:
        If miss the point will be solid half brightness.

    STATE_RESULT:
        If all ships sunk display “You Lose...” and play lose sound.
