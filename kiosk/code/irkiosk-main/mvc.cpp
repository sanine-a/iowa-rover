#include "mvc.h"


// --===== Model =====--

// initialize the model (especially the hash table)
Model::Model() : tbl(682) {
	resetCommands();
}

void Model::resetCommands() {
	for (int i=0; i<4; i++) {
		commands[i].action = Command::Action::NONE;
		commands[i].amount = 0.5;
	}
}


// --===== View =====--

// initialize the View
View::View(Model& model) : 
	txButton(model, srLamps.txBtn),
	slotBarGraphs(model),
	slotButtons(model, srLamps) 
{}

// update all view components
void View::update() {
	progBtns.update();
	progLeds.update();
	rfid.update();
	slotButtons.update();
	slotBarGraphs.update();
	txButton.update();
	srLamps.update();
}


// --===== Controller =====--

// initialize the controller
Controller::Controller() : 
	view(model), // initialize view
	// connect radio to lamps and buttons
	radio(
		model, &(view.txButton), sch, 
		view.srLamps.ready, view.srLamps.txBtn, view.srLamps.error, view.srLamps.running, 
		view.rfid
	),
	// connect rfidMonitor to the model and rfid readers
	rfidMonitor(model.tbl, view.rfid),
	// connect rfidPoller to the model and rfid readers
	rfidPoller(model, sch, view.rfid),
	// connect the programmer controller to the model, rfid readers, and programmer buttons & leds
	progController(view.progBtns, view.progLeds, view.rfid, model.tbl),
	// connect each command slot to the appropriate lamp and buttons
	slotReader1(model, 0, sch, view.rfid, 0x71, view.srLamps.rfid1, view.slotButtons),
	slotReader2(model, 1, sch, view.rfid, 0x72, view.srLamps.rfid2, view.slotButtons),
	slotReader3(model, 2, sch, view.rfid, 0x73, view.srLamps.rfid3, view.slotButtons),
	slotReader4(model, 3, sch, view.rfid, 0x74, view.srLamps.rfid4, view.slotButtons)
{}

// update all controller components
void Controller::update() {
	sch.update();
	view.update();
	progController.update();
	slotReader1.update();
	slotReader2.update();
	slotReader3.update();
	slotReader4.update();
	radio.update();
}


// --===== extern globals =====--

struct Controller controller;


// called repeatedly in loop()
void mvc_update() {
	controller.update();
}
