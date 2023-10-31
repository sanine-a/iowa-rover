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
	ShiftLamp& errLamp, ShiftLamp& runLamp, Rfid& rfid
) : 
	sch(sch), Subscriber(pub), model(model), RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, 910.0),
	errLamp(errLamp), runLamp(runLamp), rfid(rfid),
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
	}, 0);
}


void Transmitter::on(TxButtonEvent e) {
	if (running) { return; } // do nothing while the rover is already running
	Serial.println("transmitting!");
	for (int i=0; i<4; i++) {
		if (model.commands[i].action != Model::Command::Action::NONE) {
			sendCommand(i, model.commands[i].action, model.commands[i].amount);
		}
	}
	if (!sendMessage("execute", 1)) { flashError(); return; }
	Serial.println("transmitted!");
}


void Transmitter::sendCommand(int index, Model::Command::Action action, float amount) {
	if (!sendMessage("command", index)) { flashError(); return; }
	delay(300);
	if (!sendMessage("action", action)) { flashError(); return; }
	delay(300);
	if (!sendMessage("amount", amount)) { flashError(); return; }
	delay(300);
}


void Transmitter::onMessage(const char *key, const char *value) {
	Serial.print("  RX: {"); Serial.print(key); Serial.print(":"); Serial.print(value); Serial.println("}");
	if (strcmp(key, "ready") == 0) {
		unsigned int ready = atoi(value);
		if (!ready) {
			running = true;
			runLamp.turnOn();
		} else {
			running = false;
			runLamp.turnOff();
		}
	}
}


void Transmitter::flashError() {
	errLamp.turnOn();
	sch.setTimeout([this]{ errLamp.turnOff(); }, 2000);
}
