#include <Arduino.h>
#include "sr_lamp.h"
#include "pins.h"

#define OFFSET_BYTE(off) (off/8)
#define OFFSET_BIT(off)  (off%8)

ShiftRegisterPin::ShiftRegisterPin(byte* root, size_t offset)
: b(root + OFFSET_BYTE(offset)), pos(OFFSET_BIT(offset)) {}

bool ShiftRegisterPin::write1() {
	byte old = *b;
	*b |= 1 << pos;
	return old != *b;
}

bool ShiftRegisterPin::write0() {
	byte old = *b;
	*b &= ~(1 << pos);
	return old != *b;
}


ShiftLamp::ShiftLamp(ShiftLamps* lamps, size_t index) : lamps(lamps), ShiftRegisterPin(lamps->data, index) {}
void ShiftLamp::turnOn()  { if (write1()) { lamps->show(); } }
void ShiftLamp::turnOff() { if (write0()) { lamps->show(); } }


ShiftLamps::ShiftLamps() :
inc1(this, 0), dec1(this, 1),
inc2(this, 3), dec2(this, 2),
inc3(this, 4), dec3(this, 5),
inc4(this, 6), dec4(this, 7),

rfid1(this, 8),
rfid2(this, 9),
rfid3(this, 11),
rfid4(this, 10),

ready(this, 12), error(this, 13), running(this, 14), txBtn(this, 15) {
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
	shiftOut(SHIFT_DATA, SHIFT_CLOCK, MSBFIRST, data[0]);
	digitalWrite(SHIFT_LATCH, 1);
}
