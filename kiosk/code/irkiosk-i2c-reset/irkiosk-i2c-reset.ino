#include <Wire.h>

#define GATE_PIN 4

void reset(int n) {
	Wire.read();
	digitalWrite(GATE_PIN, 0);
	delay(20);
	digitalWrite(GATE_PIN, 1);
}


void setup() {
	Wire.begin(0x55);
	Wire.onReceive(reset);
	pinMode(GATE_PIN, OUTPUT);
	digitalWrite(GATE_PIN, 1);
}

void loop() {}
