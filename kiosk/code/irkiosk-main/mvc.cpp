#include "mvc.h"


// --===== Model =====--

Model::Model() : tbl(682) {}


// --===== View =====--

void View::update() {
	progBtns.update();
	progLeds.update();
	rfid.update();
}


// --===== Controller =====--

Controller::Controller() : progController(view.progBtns, view.progLeds, view.rfid, model.tbl) {

	#define HALF_SEC 500
	sch.setTimeout([this]{ view.srLamps.inc1.turnOn(); view.srLamps.show(); }, 1 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.dec1.turnOn(); view.srLamps.show(); }, 2 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.inc2.turnOn(); view.srLamps.show(); }, 3 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.dec2.turnOn(); view.srLamps.show(); }, 4 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.inc3.turnOn(); view.srLamps.show(); }, 5 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.dec3.turnOn(); view.srLamps.show(); }, 6 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.inc4.turnOn(); view.srLamps.show(); }, 7 * HALF_SEC);
	sch.setTimeout([this]{ view.srLamps.dec4.turnOn(); view.srLamps.show(); }, 8 * HALF_SEC);
}

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
