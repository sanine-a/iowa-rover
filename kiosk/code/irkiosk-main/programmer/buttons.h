#pragma once

#include "../PolledSwitch.h"
#include "../pins.h"
#include "../signal.h"
#include "../scheduler.h"


namespace Programmer {

struct ButtonState {
	typedef enum {
		PRESS, RELEASE, HOLD,
	} S;

	S forward, backward, left, right, erase;
};


class Buttons;


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

#if defined(IMPL)

Btn::Btn(int pin, Buttons& pub, ButtonState::S& state)
: PolledSwitch(pin), pub(pub), state(state), timeout(-1) {}

void Btn::onLow() {
	state = ButtonState::S::PRESS;
	timeout = pub.sch.setTimeout([this]{
		this->state = ButtonState::S::HOLD;
		pub.publish(pub.state);
	}, 500);
	pub.publish(pub.state);
}
void Btn::onHigh() {
	state = ButtonState::S::RELEASE;
	pub.sch.clearTimeout(timeout);
	pub.publish(pub.state);
}

#endif

} // end namespace Programmer
