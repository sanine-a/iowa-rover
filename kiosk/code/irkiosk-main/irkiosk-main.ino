#include <Wire.h>
#include "mvc.h"


void setup() {
	// initialize I2C bus and serial comms
	Wire.begin();
	Serial.begin(115200);
	Serial.println("c:");
}

void loop() {
	// update the model, view, and controller
	mvc_update();
}
