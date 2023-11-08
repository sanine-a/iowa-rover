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


// struct to hold and manage "view" components
// a view is essentially anything that the user can interact with (LEDs, buttons, RFID readers, etc)
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

// struct to hold and manage "controller" components
// controllers read from the Model and "view" components, and update both as appropriate
// they are where behavior lives
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
