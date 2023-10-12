#pragma once

#include <EEPROM.h>

#define FLAG_UNOCCUPIED 0x80
#define FLAG_DELETED  0x40
#define FLAG_CATEGORY 0x3f


// each slot is laid out in memory like this:
//
// flags tag0 tag1 tag2 tag3 tag4
//
// the first bit of the flags byte is 1 if the slot is unset (truly empty) and zero if it is set.
// the second bit is zero if the slot is deleted. deleted slots are treated as occupied during
// search but unoccupied during insertion.
// the 6 least significant bits of `flags` contain the category information for the tag.

struct HashSlot {
	unsigned int slot;

	int getAddress() { return 6*slot; }

	byte getFlags() {
		int address = getAddress();
		return EEPROM.read(address);
	}

	byte getCategory() {
		byte flags = getFlags();
		return flags & FLAG_CATEGORY;
	}

	RfidTag getTag() {
		int addr = getAddress();
		RfidTag tag;
		for (int i=0; i<5; i++) {
			tag[i] = EEPROM.read(addr+i+1);
		}
	}

	bool isOccupied(bool inserting) {
		byte flags = getFlags();
		if (flags & FLAG_UNOCCUPIED) {
			// a 1 in the MSB indicates an unoccupied slot
			return false;
		}

		if (inserting && (flags & FLAG_DELETED)) {
			// deleted slots are treated as unoccupied when inserting
			return false;
		}

		return true;
	}

	void setUnoccupied() {
		int addr = getAddress();
		EEPROM.update(addr, 0xff);
	}

	void setDeleted() {
		int addr = getAddress();
		EEPROM.update(addr, 0x7f);
	}

	void setCategory(byte category) {
		int addr = getAddress();
		EEPROM.update(addr, category & FLAG_CATEGORY);
	}

	void setTag(RfidTag tag) {
		int addr = getAddress();
		for (int i=0; i<5; i++) {
			EEPROM.update(addr+i+1, tag[i]);
		}
	}
};
