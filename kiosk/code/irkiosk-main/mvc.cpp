#include "mvc.h"


// --===== Model =====--

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

View::View(Model& model) : slotButtons(model, srLamps) {}

void View::update() {
	progBtns.update();
	progLeds.update();
	rfid.update();
	slotButtons.update();
}


// --===== Controller =====--

Controller::Controller() : 
	view(model),
	rfidMonitor(model.tbl, view.rfid),
	progController(view.progBtns, view.progLeds, view.rfid, model.tbl),
	slotReader1(model, 0, sch, view.rfid, 0x71, view.srLamps, view.srLamps.rfid1),
	slotReader2(model, 1, sch, view.rfid, 0x72, view.srLamps, view.srLamps.rfid2),
	slotReader3(model, 2, sch, view.rfid, 0x73, view.srLamps, view.srLamps.rfid3),
	slotReader4(model, 3, sch, view.rfid, 0x74, view.srLamps, view.srLamps.rfid4)
{}

void Controller::update() {
	view.update();
	sch.update();
	progController.update();
}


// --===== extern globals =====--

struct Controller controller;


void mvc_update() {
	controller.update();
}
