#pragma once

class PolledSwitch {
	public:
	PolledSwitch(unsigned int pin, unsigned long debounceTime=5, int mode=INPUT_PULLUP)
	: pin(pin), debouncing(false), debounceEnd(0), debounceTime(debounceTime) {
		pinMode(pin, mode);
		prevState = digitalRead(pin);
	}

	virtual void onLow() {}
	virtual void onHigh() {}

	void update() {
		if (debouncing) {
			if (millis() > debounceEnd) {
				debouncing = false;
			}
		} else {
			bool state = digitalRead(pin);
			if (prevState != state) {
				debouncing = true;
				debounceEnd = millis() + debounceTime;
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
