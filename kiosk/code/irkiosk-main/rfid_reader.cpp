#include <Wire.h>
#include "rfid_reader.h"


#define CMD_CLEAR_TAG 0x54


RfidReader::RfidReader(Publisher<struct RfidEvent> *publisher, byte addr) : publisher(publisher), addr(addr) {}

void RfidReader::clearTag() {
	Wire.beginTransmission(addr);
	Wire.write(CMD_CLEAR_TAG);
	Wire.endTransmission();
}


bool RfidReader::loadTag(RfidTag& tag) {
	Wire.requestFrom(addr, 6);
	for (int i=0; i<5; i++) {
		if (!(Wire.available())) { return false; }
		tag[i] = Wire.read();
	}

	if (!(Wire.available())) { return false; }
	byte checksum = Wire.read();
	if (tag.checksum() != checksum) { return false; } // bad checksum
	return true;
}



bool RfidReader::loadNonEmptyTag(RfidTag& tag, unsigned int max_retries) {
	for (int i=0; i<max_retries; i++) {
		if (loadTag(tag)) {
			if (!tag.isEmpty()) {
				return true;
			}
		}
	}
	return false;
}


void RfidReader::update() {
	RfidTag tag;
	struct RfidEvent e;
	if (loadNonEmptyTag(tag)) {
		clearTag();
		e.sourceAddr = addr;
		e.tag = tag;
		publisher->publish(e);
	}
}


void RfidReader::test() {
	RfidTag tag;
	Serial.print("reader 0x"); Serial.print(addr, HEX);
	if (loadTag(tag)) {
		Serial.println(": OK");
	} else {
		Serial.println(": FAIL");	
		Wire.requestFrom(addr, 6);
		Serial.print("  requested 6 bytes, received "); Serial.println(Wire.available());
		while(Wire.available()) {
			Serial.print("  0x"); Serial.println(Wire.read(), HEX);
		}
	}
}


// --===== Rfid =====--

Rfid::Rfid() : 
	rfid1(this, 0x71), rfid2(this, 0x72), 
	rfid3(this, 0x73), rfid4(this, 0x74), 
	progRfid(this, 0x70), 
	resetting(false) {}

void Rfid::test() {
	progRfid.test();
	rfid1.test();
	rfid2.test();
	rfid3.test();
	rfid4.test();
}


void Rfid::update() {
	sch.update();
	if (!resetting) {
		progRfid.update();
		rfid1.update();
		rfid2.update();
		rfid3.update();
		rfid4.update();
	}
}


void Rfid::reset() {
	Serial.println("RESET RFID");
	Wire.beginTransmission(0x55);
	Wire.write(0x00);
	Wire.endTransmission();
	resetting = true;
	sch.setTimeout([this]{ resetting = false; }, 100);
}
