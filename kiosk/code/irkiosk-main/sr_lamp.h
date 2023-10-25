#pragma once

#include <Arduino.h>


class ShiftRegisterPin {
	public:
	ShiftRegisterPin(byte* root, size_t offset);

	bool write1();
	bool write0();

	private:
	byte *b;
	byte pos;
};


struct ShiftLamps;


class ShiftLamp : public ShiftRegisterPin {
	public:
	ShiftLamp(ShiftLamps* lamps, size_t index);

	void turnOn();
	void turnOff();

	private:
	ShiftLamps* lamps;
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
