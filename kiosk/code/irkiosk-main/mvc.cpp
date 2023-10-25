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

View::View(Model& model) : 
	txButton(model, srLamps.txBtn),
	slotBarGraphs(model),
	slotButtons(model, srLamps) 
{}

void View::update() {
	progBtns.update();
	progLeds.update();
	rfid.update();
	slotButtons.update();
	slotBarGraphs.update();
	txButton.update();
}


// --===== Controller =====--

Controller::Controller() : 
	view(model),
	radio(model, &(view.txButton), sch),
	rfidMonitor(model.tbl, view.rfid),
	progController(view.progBtns, view.progLeds, view.rfid, model.tbl),
	slotReader1(model, 0, sch, view.rfid, 0x71, view.srLamps.rfid1, view.slotButtons),
	slotReader2(model, 1, sch, view.rfid, 0x72, view.srLamps.rfid2, view.slotButtons),
	slotReader3(model, 2, sch, view.rfid, 0x73, view.srLamps.rfid3, view.slotButtons),
	slotReader4(model, 3, sch, view.rfid, 0x74, view.srLamps.rfid4, view.slotButtons)
{
//	#define HALF_SEC 500
//	sch.setTimeout([this]{ Serial.println("1 on"); view.srLamps.inc1.turnOn(); }, 1 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("2 on"); view.srLamps.inc2.turnOn(); }, 2 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("3 on"); view.srLamps.inc3.turnOn(); }, 3 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("4 on"); view.srLamps.inc4.turnOn(); }, 4 * HALF_SEC);
//
//
//	sch.setTimeout([this]{ Serial.println("1 off"); view.srLamps.inc1.turnOff(); }, 2 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("2 off"); view.srLamps.inc2.turnOff(); }, 3 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("3 off"); view.srLamps.inc3.turnOff(); }, 4 * HALF_SEC);
//	sch.setTimeout([this]{ Serial.println("4 off"); view.srLamps.inc4.turnOff(); }, 5 * HALF_SEC);
}

void Controller::update() {
	view.update();
	sch.update();
	progController.update();
	slotReader1.update();
	slotReader2.update();
	slotReader3.update();
	slotReader4.update();
	//radio.update();
}


// --===== extern globals =====--

struct Controller controller;


void mvc_update() {
	controller.update();
}
