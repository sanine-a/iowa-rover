#pragma once

#include "PolledSwitch.h"
#include "pins.h"
#include "signal.h"
#include "scheduler.h"


namespace Programmer {

// published type for button events
struct ButtonState {
	typedef enum {
		PRESS, RELEASE, HOLD,
	} S;

	S forward, backward, left, right, erase;
};


class Buttons;


// individual button class
class Btn : public PolledSwitch {
	public:
	Btn(int pin, Buttons& pub, ButtonState::S& state);
	private:
	Buttons& pub;
	ButtonState::S& state;
	int timeout;
	void onLow();
	void onHigh();
};

// class to manage all buttons & publish button events
// the individual Btn objects actually do the publishing
class Buttons : public Publisher<ButtonState> {
	public:
	friend class Btn;

	Buttons() : 
	forward (PROG_BTN1, *this, state.forward),
	backward(PROG_BTN2, *this, state.backward),
	left    (PROG_BTN3, *this, state.left),
	right   (PROG_BTN4, *this, state.right),
	erase   (PROG_BTN5, *this, state.erase)
	{
		state.forward  = ButtonState::S::RELEASE;
		state.backward = ButtonState::S::RELEASE;
		state.left     = ButtonState::S::RELEASE;
		state.right    = ButtonState::S::RELEASE;
		state.erase    = ButtonState::S::RELEASE;
	}

	void update() {
		forward.update(); backward.update();
		left.update(); right.update();
		erase.update();
		sch.update();
	}

	protected:
	Scheduler sch;
	struct ButtonState state;
	Btn forward, backward, left, right, erase;
};

} // end namespace Programmer
