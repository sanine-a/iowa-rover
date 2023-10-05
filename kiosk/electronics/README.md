Iowa Science Rover - Electronics
================================

Due to the myriad inputs and outputs that need to be managed, the electronics of the kiosk have been split into
a constellation of connected boards rather than using a single monolithic board. The scheme of connections is
as follows:

[![](board-layout.png)

  * **Arduino Mega**: This is an ATmega2560-based microcontroller, used as the central processor for the kiosk.
  * **Mega Adapter**: This provides a convenient 40-pin ribbon cable connection between the Arduino Mega and the
    main board. It carries power from the main board to the Arduino.
  * **Main Board**: The heart of the kiosk. It carries the comms radio, the 5V regulator to step down from the 12V
    power supply, the ATtiny84 chip to drive the RFID programmer's RFID reader, and connectors to attach all of the
    peripheral boards to the Arduino.
  * **RFID Programmer**: This board carries an ID-12LA RFID reader and a set of buttons. It is used to configure new
    tags appropriately so that they will work with the kiosk, and it is kept inside the kiosk cabinet under normal
    circumstances. It connects to the main board with a 12-pin ribbon cable.
  * **RFID**: These four identical boards are the RFID readers used to read the tags. They each have their own ID-12LA
    reader and an ATtiny84 chip to drive them over the I2C bus. They also carry jumpers that permit setting the I2C bus
    address of each board, allowing them to be daisy-chained. They connect to each other and to the main board with
    4-pin KK connectors.
  * **Lamps**: This board drives the lamps above the RFID readers, the READY, ERROR, and RUNNING lamps, and both the
    lamp and button inside the TRANSMIT button. It is connected to the main board with an 8-pin ribbon cable.
  * **Level Buttons**: This board drives the lamps and buttons below the RFID readers (but not the bar graphs). It is
    connnected to the main board with a 16-pin ribbon cable.
  * **LEDs**: These NeoPixels are the bar graphs, and are chained together with wire-to-wire inline connectors. They
    connect to the main board with a 3-pin KK connector.
