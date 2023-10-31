#pragma once

#include "scheduler.h"
#include "signal.h"
#include "model.h"

#include "Rfid/HashTable.h"
#include "Rfid/Tag.h"

#include "programmer_buttons.h"
#include "programmer_controller.h"
#include "programmer_leds.h"

#include "sr_lamp.h"

#include "rfid_reader.h"
#include "rfid_monitor.h"
#include "slot.h"

#include "transmit.h"


struct View {
	Programmer::Buttons progBtns;
	Programmer::Leds progLeds;
	ShiftLamps srLamps;
	Rfid rfid;
	SlotButtons slotButtons;
	SlotBarGraphs slotBarGraphs;
	TxButton txButton;

	View(Model& model);
	void update();
};

struct Controller {
	Scheduler sch;
	struct View view;
	struct Model model;

	RfidMonitor rfidMonitor;
	RfidPoller rfidPoller;
	Programmer::Controller progController;
	SlotReader slotReader1, slotReader2, slotReader3, slotReader4;
	Transmitter radio;

	Controller();
	void update();
};


void mvc_update();


extern struct Controller controller;
