#include "mvc.h"


// --===== Model =====--

void Model::update() {}


// --===== View =====--

void View::update() {
	progBtns.update();
	progLeds.update();
	rfid.update();
}


// --===== Controller =====--

Controller::Controller() {
	view.rfid.attach(this);

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
	model.update();
	view.update();
	sch.update();
}


void Controller::on(struct RfidEvent e) {
	char str[16];
	e.tag.toString(str, sizeof(str));
	Serial.println("== RFID READ ==");
	Serial.print("  from: "); Serial.println(e.sourceAddr, HEX);
	Serial.print("  tag:  "); Serial.println(str);
	Serial.println();
}


// --===== extern globals =====--

struct Controller controller;


void mvc_update() {
	controller.update();
}
