#pragma once

#include <Arduino.h>
#include "Rfid/Tag.h"
#include "signal.h"


struct RfidEvent {
	byte sourceAddr;
	RfidTag tag;
};


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


class Rfid : public Publisher<struct RfidEvent> {
	public:
	Rfid();
	void test();
	void update();

	private:
	RfidReader rfid1, rfid2, rfid3, rfid4, progRfid;
};
