#include "transmit.h"

#include <Arduino.h>
#include "pins.h"


TxButton::TxButton(Model& model, ShiftLamp& lamp) :
	model(model), PolledSwitch(TX_BTN), lamp(lamp), active(false)
{}

void TxButton::update() {
	PolledSwitch::update();
	active = false;
	for (int i=0; i<4; i++) {
		if (model.commands[i].action != Model::Command::Action::NONE) {
			active = true;
		}
	}

	if (active) { lamp.turnOn(); }
	else { lamp.turnOff(); }
}


void TxButton::onLow() {
	if (!active) { return; }
	publish(TxButtonEvent::TX_PRESS);
}

void TxButton::onHigh() {}


Transmitter::Transmitter(
	Model& model, Publisher<TxButtonEvent> *pub, Scheduler& sch,
	ShiftLamp& readyLamp, ShiftLamp& txLamp, ShiftLamp& errLamp, ShiftLamp& runLamp, 
	Rfid& rfid
) : 
	sch(sch), Subscriber(pub), model(model), RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, 910.0),
	readyLamp(readyLamp), txLamp(txLamp), errLamp(errLamp), runLamp(runLamp), rfid(rfid),
	running(false)
{
	pinMode(KIOSK_SELECT, INPUT);
	Serial.println("schedule tx setup");
	sch.setTimeout([this]{ 
		setup(); 
		if (digitalRead(KIOSK_SELECT)) {
			set_addrs(KIOSK_ADDR1, ROVER_ADDR1);
		} else {
			set_addrs(KIOSK_ADDR2, ROVER_ADDR2);
		}
		this->readyLamp.turnOn();
	}, 0);
}


char command_char(Model::Command::Action a) {
	switch(a) {
	case Model::Command::Action::FORWARD:  return 'F';
	case Model::Command::Action::BACKWARD: return 'B';
	case Model::Command::Action::LEFT:     return 'L';
	case Model::Command::Action::RIGHT:    return 'R';
	case Model::Command::Action::NONE:     return 'N';
	default: return 'N';
	}
}

int command_amount(float amt) {
	return round(8.0 * amt);
}


void Transmitter::on(TxButtonEvent e) {
	if (running) { return; } // do nothing while the rover is already running
	readyLamp.turnOff();
	txLamp.turnOff();
	errLamp.turnOff();
	Serial.println("transmitting!");
	char buf[16];
	snprintf(
		buf, sizeof(buf), "%c%d%c%d%c%d%c%d",
		command_char(model.commands[0].action),
		command_amount(model.commands[0].amount),
		command_char(model.commands[1].action),
		command_amount(model.commands[1].amount),
		command_char(model.commands[2].action),
		command_amount(model.commands[2].amount),
		command_char(model.commands[3].action),
		command_amount(model.commands[3].amount)
	);
	if (!sendMessage("execute", buf)) { flashError(); }
	Serial.println("transmitted!");
}


void Transmitter::sendCommand(int index, Model::Command::Action action, float amount) {
	txLamp.turnOn();
	if (!sendMessage("command", index)) { flashError(); return; }
	delay(150);
	txLamp.turnOff();
	delay(150);
	txLamp.turnOn();
	if (!sendMessage("action", action)) { flashError(); return; }
	delay(150);
	txLamp.turnOff();
	delay(150);
	txLamp.turnOn();
	if (!sendMessage("amount", amount)) { flashError(); return; }
	delay(150);
	txLamp.turnOff();
	delay(150);
}


void Transmitter::onMessage(const char *key, const char *value) {
	Serial.print("  RX: {"); Serial.print(key); Serial.print(":"); Serial.print(value); Serial.println("}");
	if (strcmp(key, "ready") == 0) {
		unsigned int ready = atoi(value);
		if (!ready) {
			running = true;
			runLamp.turnOn();
			sch.setTimeout([this]{
				// force time out if we miss the end response
				running = false;
				runLamp.turnOff();
				readyLamp.turnOn();
			}, 30 * 1000);
		} else {
			running = false;
			runLamp.turnOff();
			readyLamp.turnOn();
		}
	}
}


void Transmitter::flashError() {
	errLamp.turnOn();
}
