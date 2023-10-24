#pragma once

#include "scheduler.h"
#include "signal.h"

#include "programmer_buttons.h"
#include "programmer_controller.h"
#include "programmer_leds.h"

#include "sr_lamp.h"

#include "rfid_reader.h"


struct Model {
	void update();
};

struct View {
	Programmer::Buttons progBtns;
	Programmer::Leds progLeds;
	ShiftLamps srLamps;
	Rfid rfid;

	void update();
};

struct Controller : public Subscriber<struct RfidEvent> {
	Scheduler sch;
	struct View view;
	struct Model model;

	Controller();
	void update();

	void on(struct RfidEvent);
};


void mvc_update();


extern struct Controller controller;
