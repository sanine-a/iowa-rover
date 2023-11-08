# irkiosk-main

The main sketch for the rover control kiosk.

## functional description

### programming board

The programing board carries a single RFID reader, and several buttons with LEDs above them. When an RFID tag is scanned
on the programmer, if the tag is currently known by the system it briefly lights the appropriate LED associated with
the tag. If it is unknown, all of the LEDs flash until one of the buttons is pressed to assign the tag to a category.
Whenever a tag is scanned, the last seen tag is stored. If the ERASE button is pressed, that tag's association is removed
from the RFID hash table and the tag can then be re-scanned to program again.


### command selection

There are four RFID readers on the main kiosk panel. When a tag is scanned on one, the associated lamp turns on, the bar graph below it lights up, and the bar graph buttons become active. The RFID readers are periodically power-cycled to check if a 
tag has been removed; if it has been, then the bar graph, buttons, and LED all turn off. While a tag is present, the bar graph
buttons can be used to increase or decrease the "amount" associated with the command. Both the command type and amount are
stored in a global `model` object for reading by other components.


### transmission

When at least one command is selected, the transmission button lights up and becomes active. When it is pressed, the commands
are strung together into a string like "F4B4L2R8", where the letter represents the command type (e.g. F is forward, L is left, etc) and the number represents the amount from 0 to 8. The program then attempts to transmit this as an `{execute:[string]}` command.
If it succeeds, the READY lamp turns off and the RUNNING lamp turns on upon receiving a return `{ready:0}` message, and the transmit button becomes inactive. When the radio receives a `{ready:1}` message back from the rover, or if 30 seconds have passed, the RUNNING lamp turns off, the READY lamp turns on, and the transmit button becomes active again. However, if the transmission fails, the ERROR lamp turns on until the next time a tranmission is attempted.


## files

  * `category.h`: stores the tag category constants
  * `CMakeLists.txt`: the build tool file for building the unit tests
  * `irkiosk-main.ino`: the main sketch file
  * `lambda.h`: helpful Lambda class for using lambda functions without the STL
  * `lily-test/`: test framework for the unit tests
  * `ll.h`: generic linked-list data structure
  * `mock/`: contains files used as mocks in the unit tests
  * `model.h`: declaration of the Model struct, used to store state information
  * `mvc.cpp`: the heart of the code, tying together the "view", "model", and "controller" components
  * `mvc.h`: definitions for the View and Controller classes
  * `pins.h`: constants mapping peripherals to specific pin numbers
  * `PolledSwitch.h`: generic polled switch class
  * `programmer_buttons.cpp`: implementation of the behavior for the programming board's buttons
  * `programmer_buttons.h`:   declaration of Programmer::ButtonState and Programmer::Buttons
  * `programmer_controller.cpp`: Programmer::Controller implementation 
  * `programmer_controller.h`: Programmer::Controller declaration, implementing the programmer behavior
  * `programmer_leds.h`: header-only implementation of Programmer::Leds, used to drive the programming board's LEDs
  * `radioserial.h`: port of the SMM serial protocol to RFM69HCW modules
  * `README.md`: the file you're reading right now c:
  * `Rfid/`:
    * `Rfid/HashSlot.h`: hash table slot helper
    * `Rfid/HashTable.h`: implementation of an open-addressed hash table over EEPROM
    * `Rfid/Tag.h`: RfidTag struct
  * `rfid_monitor.cpp`: implement RfidMonitor and RfidPoller
  * `rfid_monitor.h`: declare RfidMonitor (prints RFID events to serial) and RfidPoller (reboots the RFID modules each second)
  * `rfid_reader.cpp`: implement RfidReader class and Rfid struct
  * `rfid_reader.h`: declare RfidReader (manages a single RFID reader over I2C) and Rfid (which collects all of the RFID readers into a single handy object)
  * `scheduler.h`: javascript-style lambda-based event scheduling
  * `signal.h`: implementation of the Observer pattern
  * `slot.cpp`: code for the lamp, bar graph, and bar graph buttons of the individual command slots
  * `slot.h`: declarations for `slot.cpp`
  * `sr_lamp.cpp`: implement ShiftLamp class and ShiftLamps struct
  * `sr_lamp.h`: declare ShiftLamp (used to manage a single lamp on the shift registers) and ShiftLamps (used to manage all ShiftLamps)
  * `test`: main code for the unit tests
  * `transmit.cpp`: implement TxButton and Transmitter classes
  * `transmit.h`: declare TxButton (manages the transmit button) and Transmitter (manages the radio communications)
