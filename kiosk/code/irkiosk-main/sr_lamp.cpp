#include <Arduino.h>
#include "sr_lamp.h"
#include "pins.h"

#define OFFSET_BYTE(off) (off/8)
#define OFFSET_BIT(off)  (off%8)

ShiftRegisterPin::ShiftRegisterPin(byte* root, size_t offset)
: b(root + OFFSET_BYTE(offset)), pos(OFFSET_BIT(offset)) {}

void ShiftRegisterPin::write1() {
	*b |= 1 << pos;
}

void ShiftRegisterPin::write0() {
	*b &= ~(1 << pos);
}


ShiftLamp::ShiftLamp(byte* root, size_t index) : ShiftRegisterPin(root, index) {}
void ShiftLamp::turnOn() { write1(); }
void ShiftLamp::turnOff() { write0(); }


ShiftLamps::ShiftLamps() :
inc1(data, 0), dec1(data, 1),
inc2(data, 2), dec2(data, 3),
inc3(data, 4), dec3(data, 5),
inc4(data, 6), dec4(data, 7),

rfid1(data, 8),
rfid2(data, 9),
rfid3(data, 10),
rfid4(data, 11),

ready(data, 12), error(data, 13), running(data, 14), txBtn(data, 15) {
	pinMode(SHIFT_DATA, OUTPUT);
	pinMode(SHIFT_CLOCK, OUTPUT);
	pinMode(SHIFT_LATCH, OUTPUT);
	clear();
	show();
}


void ShiftLamps::clear() {
	memset(data, 0, sizeof(data));
}


void ShiftLamps::show() {
	digitalWrite(SHIFT_LATCH, 0);
	shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, data[1]);
	shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, data[2]);
	digitalWrite(SHIFT_LATCH, 1);
}
