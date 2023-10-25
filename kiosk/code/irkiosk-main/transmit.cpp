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


Transmitter::Transmitter(Model& model, Publisher<TxButtonEvent> *pub, Scheduler& sch) : 
	Subscriber(pub), model(model), RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, 910.0)
{
	sch.setTimeout([this]{ setup(); }, 0);
}


void Transmitter::on(TxButtonEvent e) {
	Serial.println("transmitting!");
	for (int i=0; i<4; i++) {
		if (model.commands[i].action != Model::Command::Action::NONE) {
			sendCommand(i, model.commands[i].action, model.commands[i].amount);
		}
	}
	sendMessage("execute", 1);
	Serial.println("transmitted!");
}


void Transmitter::sendCommand(int index, Model::Command::Action action, float amount) {
	sendMessage("command", index);
	sendMessage("action", action);
	sendMessage("amount", amount);
}


void Transmitter::onMessage(const char *key, const char *value) {
}
