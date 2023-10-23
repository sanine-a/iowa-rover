#define IMPL
#include <Wire.h>
#include "PolledSwitch.h"
#include "pins.h"

#include "scheduler.h"
#include "signal.h"
#include "programmer/leds.h"
#include "programmer/buttons.h"


Programmer::Buttons progBtns;

class Rx : public Subscriber<Programmer::ButtonState> {
	public:
	Rx() : Subscriber(&progBtns) {}
	void on(Programmer::ButtonState state) {
		Serial.print("forward: ");  Serial.println(state.forward);
		Serial.print("backward: "); Serial.println(state.backward);
		Serial.print("left: ");     Serial.println(state.left);
		Serial.print("right: ");    Serial.println(state.right);
		Serial.print("erase: ");    Serial.println(state.erase);
		Serial.println();
	}
} rx;


void setup() {
	Serial.begin(115200);
}

void loop() {
	progBtns.update();
}
