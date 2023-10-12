#pragma once

#include <EEPROM.h>
#include <Wire.h>

#include "Tag.h"



struct RfidReader {
	byte address;

	// get a read tag from the attiny84
	// returns true if a non-empty tag was received successfully, and false in any other case
	// in all cases, do NOT count on the data in `tag` being unmodified!
	bool rxTag(RfidTag& tag) {
		Wire.requestFrom(address, 6);

		// get the address bytes
		for (int i=0; i<5; i++) {
			if (!(Wire.available())) {
				return false; // not enough address bytes
			}
			tag.tagData[i] = Wire.read();
		}

		// get the checksum byte
		if (!(Wire.available())) {
			return false; // no checksum byte
		}
		byte checksum = Wire.read();
		if (tag.checksum() != checksum) {
			return false; // bad checksum
		}

		// tag rx'd successfully; clear remote so we don't rx it again
		clear();

		// check if tag is empty
		if (tag.isEmpty()) {
			return false;
		}

		// we have a good tag! c:
		return true;
	}


	void clear() {
		Wire.beginTransmission(address);
		Wire.write(0x54); // CLEAR_TAG command
		Wire.endTransmission();
	}
};


class EepromHashTable {
	public:
	static const byte CATEGORY_UNKNOWN = 0xff;

	byte get_tag(RfidTag tag) {
		int hash = hash_tag(tag);
		if (!hash_is_occupied(hash)) {
			// nothing at expected hash
		}
	}

	private:
	struct RfidCategoryTag {
		byte category;
		RfidTag tag;
	};

	const int TBL_SIZE = 682;
	int hash_tag(RfidTag tag) {
		// split into segments of 10 bits each
		int a = (tag[0] << 2) | (tag[1] >> 6);
		int b = (tag[1] << 4) | (tag[2] >> 4);
		int c = (tag[2] << 6) | (tag[3] >> 2);
		int d = (tag[3] << 8) | (tag[4] >> 0);

		// sum segments, mod table size
		int sum = (a + b + c + d) % TBL_SIZE;

		return sum;
	}

	// convert a hash id to to a physical EEPROM address
	inline int hash_to_address(int h) {
		return sizeof(struct RfidCategoryTag) * h;
	}
};

