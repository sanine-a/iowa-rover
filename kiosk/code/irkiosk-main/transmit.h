#pragma once

#include <Arduino.h>
#include "signal.h"
#include "model.h"
#include "PolledSwitch.h"
#include "sr_lamp.h"
#include "radioserial.h"
#include "scheduler.h"

typedef enum { TX_PRESS } TxButtonEvent;

class TxButton : public PolledSwitch, public Publisher<TxButtonEvent> {
	public:
	TxButton(Model& model, ShiftLamp& lamp);
	void update();
	private:
	bool active;
	Model& model;
	ShiftLamp& lamp;
	void onLow();
	void onHigh();
};


class Transmitter : public Subscriber<TxButtonEvent>, public RadioSerial {
	public:
	Transmitter(Model& model, Publisher<TxButtonEvent> *publisher, Scheduler& sch);
	void on(TxButtonEvent e);
	void onMessage(const char *key, const char *value);
	private:
	Model& model;
	void sendCommand(int index, Model::Command::Action action, float amount);
};
