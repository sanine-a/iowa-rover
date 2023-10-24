#pragma once

#include "signal.h"
#include "scheduler.h"
#include "programmer_leds.h"
#include "programmer_buttons.h"
#include "rfid_reader.h"
#include "Rfid/HashTable.h"


namespace Programmer {

class Controller : public Subscriber<ButtonState>, public Subscriber<struct RfidEvent> {
	public:
	Controller(Buttons &buttons, Leds &leds, Rfid &rfid, HashTable& tbl);
	void on(ButtonState state);
	void on(struct RfidEvent e);
	void update();

	private:
	Leds &leds;
	Scheduler sch;
	HashTable& tbl;

	bool resetting;
	int resetTimeout;
	void initiateReset();

	bool training;
	void train(uint8_t category);

	RfidTag tag;
};

};
