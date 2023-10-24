#include "programmer_controller.h"
#include "programmer_buttons.h"

using namespace Programmer;


Controller::Controller(Buttons &buttons, Leds &leds) 
: Subscriber(&buttons), leds(leds), resetTimeout(0), resetting(false) {}


#define H ButtonState::S::HOLD
#define o ButtonState::S::RELEASE
#define P ButtonState::S::PRESS

#define MATCH_STATE(state, f, b, l, r, e) \
	(state.forward  == f) && \
	(state.backward == b) && \
	(state.left     == l) && \
	(state.right    == r) && \
	(state.erase    == e)

char s2char(ButtonState::S s) {
	if (s == H) return 'H';
	if (s == P) return 'P';
	if (s == o) return '_';
	return '?';
}

void Controller::on(ButtonState state) {
	Serial.print("[ ");
	Serial.print(s2char(state.forward));  Serial.print(", ");
	Serial.print(s2char(state.backward)); Serial.print(", ");
	Serial.print(s2char(state.left));     Serial.print(", ");
	Serial.print(s2char(state.right));    Serial.print(", ");
	Serial.print(s2char(state.erase));    Serial.println(" ]");

	if (MATCH_STATE(state, H, o, o, o, H)) {
		initiateReset();
	} else if (resetting) {
		resetting = false;
		sch.clearTimeout(resetTimeout);
	}
}


void Controller::update() {
	sch.update();
}


void Controller::initiateReset() {
	resetting = true;
	resetTimeout = sch.setTimeout([this]{
		leds.lightForward();
		resetTimeout = sch.setTimeout([this]{
			leds.lightBackward();
			resetTimeout = sch.setTimeout([this]{
				leds.lightLeft();
				resetTimeout = sch.setTimeout([this]{
					leds.lightRight();
					resetTimeout = sch.setTimeout([this]{
						resetting = false;
						Serial.println("RESET!!!!");
						leds.clearAll();
					}, 2000);
				}, 2000);
			}, 2000);
		}, 2000);
	}, 2000);
}
