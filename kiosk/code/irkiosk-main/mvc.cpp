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
	Serial.println("view> 1");
	progBtns.update();
	Serial.println("view> 2");
	progLeds.update();
	Serial.println("view> 3");
	rfid.update();
	Serial.println("view> 4");
	slotButtons.update();
	Serial.println("view> 5");
	slotBarGraphs.update();
	Serial.println("view> 6");
	txButton.update();
	Serial.println("view> 7");
}


// --===== Controller =====--

Controller::Controller() : 
	view(model),
	radio(model, &(view.txButton), sch, view.srLamps.error, view.srLamps.running, view.rfid),
	rfidMonitor(model.tbl, view.rfid),
	rfidPoller(model, sch, view.rfid),
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
	Serial.println("mvc> 1");
	sch.update();
	Serial.println("mvc> 2");
	view.update();
	Serial.println("mvc> 3");
	progController.update();
	Serial.println("mvc> 4");
	slotReader1.update();
	Serial.println("mvc> 5");
	slotReader2.update();
	Serial.println("mvc> 6");
	slotReader3.update();
	Serial.println("mvc> 7");
	slotReader4.update();
	Serial.println("mvc> 8");
	radio.update();
	Serial.println("mvc> 9");
}


// --===== extern globals =====--

struct Controller controller;


void mvc_update() {
	controller.update();
}
