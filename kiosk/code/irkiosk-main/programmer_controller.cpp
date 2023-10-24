#include "programmer_controller.h"
#include "programmer_buttons.h"
#include "rfid_reader.h"
#include "Rfid/HashTable.h"
#include "category.h"

using namespace Programmer;


Controller::Controller(Buttons &buttons, Leds &leds, Rfid& rfid, HashTable& tbl) 
: Subscriber<ButtonState>::Subscriber(&buttons), Subscriber<RfidEvent>::Subscriber(&rfid), 
leds(leds), resetTimeout(0), resetting(false), training(false),
tag(0, 0, 0, 0, 0)
{}


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

void Controller::train(uint8_t category) {
	if (!training) { return; }
	training = false;
	tbl.storeTag(tag, category);
	leds.clearAll();
}


void Controller::on(ButtonState state) {
	Serial.print("[ ");
	Serial.print(s2char(state.forward));  Serial.print(", ");
	Serial.print(s2char(state.backward)); Serial.print(", ");
	Serial.print(s2char(state.left));     Serial.print(", ");
	Serial.print(s2char(state.right));    Serial.print(", ");
	Serial.print(s2char(state.erase));    Serial.println(" ]");

	if (MATCH_STATE(state, o, o, o, o, P)) {
		// delete last scanned tag
		tbl.deleteTag(tag);
	}

	if (training) {
		if (MATCH_STATE(state, P, o, o, o, o))      { train(CATEGORY_FORWARD);  return; }
		else if (MATCH_STATE(state, o, P, o, o, o)) { train(CATEGORY_BACKWARD); return; }
		else if (MATCH_STATE(state, o, o, P, o, o)) { train(CATEGORY_LEFT);     return; }
		else if (MATCH_STATE(state, o, o, o, P, o)) { train(CATEGORY_RIGHT);    return; }
	}

	if (MATCH_STATE(state, H, o, o, o, H)) {
		initiateReset();
		return;
	} else if (resetting) {
		resetting = false;
		sch.clearTimeout(resetTimeout);
		return;
	}
}


void Controller::on(struct RfidEvent e) {
	if (e.sourceAddr != 0x70) { return; }
	tag = e.tag;

	uint8_t category = tbl.findTag(e.tag);
	if (category == CATEGORY_UNKNOWN) {
		training = true;
		leds.flashAll();
	} else {
		leds.clearAll();
		switch (category) {
		case CATEGORY_FORWARD:
			leds.lightForward();
			sch.setTimeout([this]{ leds.clearForward(); }, 1000);
			break;

		case CATEGORY_BACKWARD:
			leds.lightBackward();
			sch.setTimeout([this]{ leds.clearBackward(); }, 1000);
			break;

		case CATEGORY_LEFT:
			leds.lightLeft();
			sch.setTimeout([this]{ leds.clearLeft(); }, 1000);
			break;

		case CATEGORY_RIGHT:
			leds.lightRight();
			sch.setTimeout([this]{ leds.clearRight(); }, 1000);
			break;

		default:
			return;
		}
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
