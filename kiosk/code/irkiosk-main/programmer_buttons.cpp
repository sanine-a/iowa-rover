#include "programmer_buttons.h"

using namespace Programmer;


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
