#pragma once

#include <Arduino.h>

// a simple polled switch class
class PolledSwitch {
	public:
	PolledSwitch(unsigned int pin, unsigned long debounceTime=5, int mode=INPUT_PULLUP)
	: pin(pin), debouncing(false), debounceEnd(0), debounceTime(debounceTime) {
		pinMode(pin, mode);
		prevState = digitalRead(pin);
	}

	// override these functions to implement switch behavior
	virtual void onLow() {}
	virtual void onHigh() {}

	// update the state of the button
	void update() {
		if (debouncing) {
			// check if we should be done debouncing
			if (millis() > debounceEnd) {
				debouncing = false;
			}
		} else {
			// check if the state has changed
			bool state = digitalRead(pin);
			if (prevState != state) {
				debouncing = true;
				debounceEnd = millis() + debounceTime;
				// call appropriate "callback"
				if (state == false) { onLow(); }
				else { onHigh(); }
			}
			prevState = state;
		}
	}

	private:
	unsigned int pin;
	bool debouncing;
	bool prevState;
	unsigned long debounceEnd, debounceTime;
};
