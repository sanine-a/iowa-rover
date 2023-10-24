#include <Wire.h>
#include "mvc.h"


void setup() {
	Wire.begin();
	Serial.begin(115200);
	Serial.println("c:");
}

void loop() {
	mvc_update();
}
