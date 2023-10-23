#pragma once

#include "../signal.h"
#include "../scheduler.h"
#include "leds.h"
#include "buttons.h"


#define STATE_MATCH(state, forward, backward, left, right, erase) \
	(state.forward  == forward)  && \
	(state.backward == backward) && \
	(state.left     == left)     && \
	(state.right    == right)    && \
	(state.erase    == erase)

namespace Programmer {

using namespace ButtonState::S;

class Controller : public Subscriber<ButtonState> {
	public:
	Controller(Buttons& buttons, Leds& leds) : Subscriber(*buttons) {}

	void on(ButtonState state) {
		if (MATCH_STATE(state, HOLD, RELEASE, RELEASE, RELEASE, HOLD)) {
			// do reset
		} 
	}
};

};
