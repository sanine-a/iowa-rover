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


void ShiftRegisterPin::toggle() {
	*b ^= 1 << pos;
}


ShiftLamp::ShiftLamp(Scheduler& sch, ShiftLamps* lamps, size_t index) 
: sch(sch), lamps(lamps), ShiftRegisterPin(lamps->data, index) {}
void ShiftLamp::turnOn() { 
	sch.clearInterval(flashInterval);
	if (write1()) { lamps->show(); } 
}
void ShiftLamp::turnOff() { 
	sch.clearInterval(flashInterval);
	if (write0()) { lamps->show(); } 
}
void ShiftLamp::flash(unsigned int period, unsigned int duration) {
	sch.clearInterval(flashInterval);
	flashInterval = sch.setInterval([this] {
		toggle(); lamps->show();
	}, period/2);
	if (duration != 0) {
		sch.setTimeout([this] { turnOff(); }, duration);
	}
}


ShiftLamps::ShiftLamps() :
inc1(sch, this, 0), dec1(sch, this, 1),
inc2(sch, this, 3), dec2(sch, this, 2),
inc3(sch, this, 4), dec3(sch, this, 5),
inc4(sch, this, 6), dec4(sch, this, 7),

rfid1(sch, this, 8),
rfid2(sch, this, 9),
rfid3(sch, this, 11),
rfid4(sch, this, 10),

ready(sch, this, 12), error(sch, this, 13), 
running(sch, this, 14), txBtn(sch, this, 15) {
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


void ShiftLamps::update() {
	sch.update();
}
