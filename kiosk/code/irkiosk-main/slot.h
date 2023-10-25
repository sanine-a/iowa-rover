#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "model.h"
#include "signal.h"
#include "scheduler.h"
#include "rfid_reader.h"
#include "sr_lamp.h"
#include "PolledSwitch.h"


struct SlotButtonEvent {
	uint8_t sourceSlot;
	typedef enum Button {
		PUSH_INCREMENT, PUSH_DECREMENT
	};
	Button btn;
};


struct SlotButtons;



class SlotReader : public Subscriber<RfidEvent>, public Subscriber<SlotButtonEvent> {
	public:
	SlotReader(
		Model& model, unsigned int commandIndex, Scheduler& sch, 
		Rfid& rfid, byte addr, 
		ShiftLamp& lamp,
		SlotButtons& slotButtons
	);
	SlotReader(Model& model, Scheduler& sch, Rfid& rfid, ShiftLamp& lamp);
	void on(RfidEvent e);
	void on(SlotButtonEvent e);
	void update();

	private:
	Model& model;
	ShiftLamp& lamp;
	Scheduler& sch;
	byte addr;
	unsigned int commandIndex;
};


class SlotButton : public PolledSwitch {
	public:
	SlotButton(
		Model& model, Publisher<SlotButtonEvent> *publisher,
		unsigned int slotIndex, SlotButtonEvent::Button buttonType, 
		unsigned int buttonPin, ShiftLamp& lamp
	);
	void update();
	
	private:
	Model& model;
	Publisher<SlotButtonEvent> *publisher;
	SlotButtonEvent::Button buttonType;
	unsigned int slotIndex;
	ShiftLamp& lamp;
	Model::Command::Action action;

	void onLow();
	void onHigh();
};


struct SlotButtons : public Publisher<SlotButtonEvent> {
	SlotButton inc1, dec1;
	SlotButton inc2, dec2;
	SlotButton inc3, dec3;
	SlotButton inc4, dec4;
	SlotButtons(Model& model, ShiftLamps& lamps);
	void update();
};


class SlotBarGraph {
	public:
	SlotBarGraph(Model& model, unsigned int slotIndex, Adafruit_NeoPixel& strip);
	void update();

	private:
	Model& model;
	Adafruit_NeoPixel& strip;
	unsigned int slotIndex;
	void showAmount(unsigned int amount);
	void clear();
};


struct SlotBarGraphs {
	Adafruit_NeoPixel strip;
	SlotBarGraph bar1, bar2, bar3, bar4;
	SlotBarGraphs(Model& model);
	void update();
};
