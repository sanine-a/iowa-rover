#include "mvc.h"


void setup() {
	Serial.begin(115200);
}

void loop() {
	mvc_update();
}
