Battleships game plan:


Game setup:
    Play paper scissors rock to see who is player one (starts the game)
        State One: choosing your option
            First display instructions: “Use navswitch to choose your option press down to select
            it.”

            Next have P, S and R be selectable whilst also waiting to recieve from the opponent,
            display “Waiting for opponent” once chosen.

        State Two: computing the winner
            When both have selected compute result and display Draw and repeat or Win or Lose
            along with sounds to represent each and designate player name as player one or two
            depending on the result.

            Next display “You are player ***” when complete.

    Place your battle ships:
        First display instructions: “use the navswitch to move your ships into position, press down
        to place your ships.”

        Next have blinking lines of lights for ship to be placed and solid lines for ships already
        placed.

        Once all ships have been placed save this as your defence screen and display “Waiting for
        opponent” until opponent is finished.

Attacking state:
    First display instructions: “Choose a place to attack using the navswitch, press down to fire.”

    State One: Setup
        On first turn: start with a blank board and a blinking single light to represent the attack co-
        ordinate. Keep a record of the board so it can be recalled when the next attack turn arrives.

        On further turns: start with the current attack board and a single blinking light to represent
        the attack co-ordinate.

    State two: When attack coordinate is chosen
        State One: Validate (ANDY)
            Check if co-ordinate has been used already, and reject if so.

            If rejected, display “You have already attacked there” and make error sound, return to
            state one.

            Else transmit the co-ordinate to the opponent and await response, (if wait is longer
            than 3 seconds re-transmit.)

        State Two: display result (REI)
            If hit-not sink display “Hit!!!” and make hit sound, this point will now be a solid light
            on the attack screen with full brightness.

            If hit-sink display “Ship Sunk!!!” and make sink sound, the entire ship that has been
            sunk will now blink with full brightness for 3 seconds then remain solid with full
            brightness.


            If Miss display “miss...” and make miss sound, this point will now be a solid light on
            the attack screen with half brightness.

        State Three: check game status
            If all ships sunk display “You win!!!” and play win sound.

            If not display “Player ***’s turn” and move to defending state.

Defending state:
    First display “Waiting for opponent to make their move” whilst waiting to receive.

    State One: setup
        Display the defence screen in its current state and wait for an attack to be recieved.
        Wait for 5 seconds and re-display message if no attack has been received.

    State Two: Attack received
        check if attack is a hit, a hit-sink, or a miss and transmit the result. Also display “Hit...”,
        “Miss!!!”, or “Sink...” along with correct sound.

        If Hit-not sink: this point will not be a blinking solid with full brightness

        If Sink: the whole ship will blink for 3 seconds then become solid half brightness.

        If miss the point will be solid half brightness.

    State Three: check game status
        If all ships sunk display “You Lose...” and play lose sound.

        If not display “Player ***’s turn” and move to attacking state.
