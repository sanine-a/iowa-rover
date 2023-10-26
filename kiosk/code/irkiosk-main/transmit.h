#pragma once

#include <Arduino.h>
#include "signal.h"
#include "model.h"
#include "PolledSwitch.h"
#include "sr_lamp.h"
#include "radioserial.h"
#include "scheduler.h"

#define KIOSK_ADDR1 0x11
#define ROVER_ADDR1 0x12
#define KIOSK_ADDR2 0x21
#define ROVER_ADDR2 0x22

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
	Transmitter(Model& model, Publisher<TxButtonEvent> *publisher, Scheduler& sch, ShiftLamp& errLamp, ShiftLamp& runLamp);
	void on(TxButtonEvent e);
	void onMessage(const char *key, const char *value);
	private:
	Scheduler& sch;
	Model& model;
	ShiftLamp &errLamp, &runLamp;
	bool running;
	void sendCommand(int index, Model::Command::Action action, float amount);
	void flashError();
};
