#include <avr/wdt.h>
#include <Wire.h>
#include "mvc.h"


void setup() {
	// initialize I2C bus and serial comms
	Wire.begin();
	Serial.begin(115200);
	Serial.println("c:");

  // configure watchdog timer with 2 second timeout
  wdt_disable();
  delay(3000);
  wdt_enable(WDTO_2S);
}

void loop() {
	// update the model, view, and controller
	mvc_update();
  // reset watchdog
  wdt_reset();
}
