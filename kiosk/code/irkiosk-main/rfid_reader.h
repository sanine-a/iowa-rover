#pragma once

#include <Arduino.h>
#include "Rfid/Tag.h"
#include "signal.h"
#include "scheduler.h"


// type for RFID events
struct RfidEvent {
	byte sourceAddr;
	RfidTag tag;
};


// single RFID module manager
class RfidReader {
	public:
	RfidReader(Publisher<struct RfidEvent> *publisher, byte addr);
	void test();
	void update();

	private:
	Publisher<struct RfidEvent> *publisher;
	byte addr;
	void clearTag();
	bool loadTag(RfidTag& tag);
	bool loadNonEmptyTag(RfidTag& tag, unsigned int max_retries=3);
};


// class to store all RFID modules
class Rfid : public Publisher<struct RfidEvent> {
	public:
	Rfid();
	void test();
	void update();
	void reset();

	private:
	RfidReader rfid1, rfid2, rfid3, rfid4, progRfid;
	Scheduler sch;
	bool resetting;
};
