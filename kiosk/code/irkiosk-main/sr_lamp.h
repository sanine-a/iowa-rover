#pragma once

#include <Arduino.h>
#include "scheduler.h"


// manage a bit from an external pointer
class ShiftRegisterPin {
	public:
	ShiftRegisterPin(byte* root, size_t offset);

	bool write1();
	bool write0();
	void toggle();

	private:
	byte *b;
	byte pos;
};


struct ShiftLamps;


// add lamp-specific functionality over ShiftRegisterPin
class ShiftLamp : public ShiftRegisterPin {
	public:
	ShiftLamp(Scheduler& sch, ShiftLamps* lamps, size_t index);

	void turnOn();
	void turnOff();
	void flash(unsigned int period, unsigned int duration=0);

	private:
	Scheduler& sch;
	int flashInterval;
	ShiftLamps* lamps;
};


// manage all lamps
struct ShiftLamps {
	byte data[2];
	Scheduler sch;
	
	ShiftLamp inc1, dec1;
	ShiftLamp inc2, dec2;
	ShiftLamp inc3, dec3;
	ShiftLamp inc4, dec4;

	ShiftLamp rfid1, rfid2, rfid3, rfid4;
	ShiftLamp ready, error, running, txBtn;

	ShiftLamps();
	void clear();
	void show();
	void update();
};
