#pragma once

#include <Arduino.h>


class ShiftRegisterPin {
	public:
	ShiftRegisterPin(byte* root, size_t offset);

	void write1();
	void write0();

	private:
	byte *b;
	byte pos;
};


class ShiftLamp : public ShiftRegisterPin {
	public:
	ShiftLamp(byte* root, size_t index);

	void turnOn();
	void turnOff();
};


struct ShiftLamps {
	byte data[2];
	
	ShiftLamp inc1, dec1;
	ShiftLamp inc2, dec2;
	ShiftLamp inc3, dec3;
	ShiftLamp inc4, dec4;

	ShiftLamp rfid1, rfid2, rfid3, rfid4;
	ShiftLamp ready, error, running, txBtn;

	ShiftLamps();
	void clear();
	void show();
};
