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


void Transmitter::on(TxButtonEvent e) {
	if (running) { return; } // do nothing while the rover is already running
	readyLamp.turnOff();
	errLamp.turnOff();
	txLamp.turnOff();
	Serial.println("transmitting!");
	int idx = 0;
	for (int i=0; i<4; i++) {
		if (model.commands[i].action != Model::Command::Action::NONE) {
			sendCommand(idx, model.commands[i].action, model.commands[i].amount);
			idx += 1;
		}
	}
	txLamp.turnOn();
	if (!sendMessage("execute", 1)) { flashError(); return; }
	delay(150);
	txLamp.turnOff();
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
