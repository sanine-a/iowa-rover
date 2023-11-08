# ir-rover

This is the code that is loaded onto each of the rovers.

## overview

This sketch waits for radio communications from the kiosk. When it receives an `execute` message, it parses
the entire sequence of commands it should execute out of the message value. The value contains four pairs like
`[COMMAND_CHAR][COMMAND_AMOUNT]`, where `COMMAND_CHAR` can be any of

  * `'F'`: move forward
  * `'B'`: move backward
  * `'L'`: turn left
  * `'R'`: turn right
  * `'N'`: empty slot, ignore

`COMMAND_AMOUNT` is a single-digit integer in the range `[0, 8]`, where 0 represents no movement and 
8 represents a "full" movement.

Once the rover begins executing a command sequence, it sends a `{ready:0}` message to the kiosk to indicate that it is busy.
When it finishes, it sends a `{ready:1}` message to indicate that it is finished and prepared to accept new transmissions.

The on-board switch determines which radio address it will listen on, and which address it will listen for transmissions from.
When in the 1 position, the rover's address is `0x12` and the expected kiosk address is `0x11`. When in the 2 position, the rover's
address is `0x22` and the expected kiosk address is `0x21`.

The on-board potentiometer determines how far a full turn is. The maximum turn time is 3000 ms, and the trimpot can be adjusted to
set the turn time to anywhere from 0 ms to 3000 ms.

Both the switch and the potentiometer are only read at boot, so if they have been adjusted, you must press the RESET button on
the rover's Metro Mini to update them.


## files

  * `README.md`: the file you're currently reading c:
  * `ir-rover.ino`: the main sketch file, and where all of the logic happens
  * `motor.h`: H-bridge motor controller class
  * `pins.h`: constants mapping pins to peripherals
  * `radioserial.h`: implement the SMM serial protocol over RFM69HCW radio modules
