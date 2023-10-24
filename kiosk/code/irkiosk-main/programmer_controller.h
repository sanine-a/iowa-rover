#pragma once

#include "signal.h"
#include "scheduler.h"
#include "programmer_leds.h"
#include "programmer_buttons.h"


namespace Programmer {

class Controller : public Subscriber<ButtonState> {
	public:
	Controller(Buttons &buttons, Leds &leds);
	void on(ButtonState state);
	void update();

	private:
	Leds &leds;
	Scheduler sch;

	bool resetting;
	int resetTimeout;
	void initiateReset();
};

};
