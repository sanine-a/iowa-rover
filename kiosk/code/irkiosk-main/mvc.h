#pragma once

#include "scheduler.h"
#include "signal.h"

#include "Rfid/HashTable.h"
#include "Rfid/Tag.h"

#include "programmer_buttons.h"
#include "programmer_controller.h"
#include "programmer_leds.h"

#include "sr_lamp.h"

#include "rfid_reader.h"


struct Model {
	struct Command {
		typedef enum {
			FORWARD, BACKWARD,
			LEFT, RIGHT, NONE
		} Action;
		Action action;
		float amount;
	};
	struct Command commands[4];

	HashTable tbl;

	Model();
};

struct View {
	Programmer::Buttons progBtns;
	Programmer::Leds progLeds;
	ShiftLamps srLamps;
	Rfid rfid;

	void update();
};

struct Controller {
	Scheduler sch;
	struct View view;
	struct Model model;

	Programmer::Controller progController;

	Controller();
	void update();
};


void mvc_update();


extern struct Controller controller;
