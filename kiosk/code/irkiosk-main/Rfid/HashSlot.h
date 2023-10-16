#pragma once

#include <EEPROM.h>
#include "Tag.h"

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
	unsigned int slot_index;

	unsigned int address() { return 6*slot_index; }

	// check if a slot is occupied
	// returns true if the cell is occupied AND undeleted when inserting
	// returns true if the cell is occupied even if deleted when searching
	bool isOccupied(bool searching) {
		uint8_t flag = EEPROM.read(address());
		if (flag & FLAG_UNOCCUPIED) {
			// the slot is unoccupied!
			return false;
		}

		if (!searching && (flag & FLAG_DELETED)) {
			return false;
		}

		return true;
	}

	// check if a slot matches a particular tag
	bool matchesTag(RfidTag tag) {
		unsigned int addr = address();
		for (int i=0; i<5; i++) {
			if (EEPROM.read(addr+i+1) != tag[i]) { return false; }
		}
		return true;
	}
};
