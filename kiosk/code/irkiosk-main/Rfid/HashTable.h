#pragma once

#include "Tag.h"
#include "HashSlot.h"

#define CATEGORY_UNKNOWN 0xff

class HashTable {
	public:
	HashTable(size_t size) : tbl_size(size) {}

	// get the category of a tag, or CATEGORY_UNKNOWN if not found
	uint8_t findTag(RfidTag tag) {
		struct HashSlot slot;
		if (!findSlot(slot, tag, true)) {
			return CATEGORY_UNKNOWN;
		}
		return slot.getCategory();
	}

	// store a new tag with its category in the table
	// note that categories can only be 6 bits long -- the upper two bits will be ignored!
	void storeTag(RfidTag tag, uint8_t category) {
		struct HashSlot slot;
		findSlot(slot, tag, false);

		slot.setOccupied(true);
		slot.setDeleted(false);
		slot.setCategory(category);
		slot.setTag(tag);
	}

	// delete a tag from the table. if the tag does not exist, does nothing
	void deleteTag(RfidTag tag) {
		struct HashSlot slot;
		if (!findSlot(slot, tag, true)) {
			return;
		}

		slot.setDeleted(true);
	}

	private:
	size_t tbl_size;

	unsigned int hash(RfidTag tag) {
		unsigned int a = ((tag[0] << 2) | (tag[1] >> 6)) & 0x3ff;
		unsigned int b = ((tag[1] << 4) | (tag[2] >> 4)) & 0x3ff;
		unsigned int c = ((tag[2] << 6) | (tag[3] >> 2)) & 0x3ff;
		unsigned int d = ((tag[3] << 8) | (tag[4] >> 0)) & 0x3ff;

		return (a+b+c+d) % tbl_size;
	}

	bool findSlot(struct HashSlot& slot, RfidTag tag, bool searching) {
		slot.slot_index = hash(tag);
		while (slot.isOccupied(searching)) {
			if ((!slot.isDeleted()) && slot.matchesTag(tag)) { return true; }
			slot.increment(tbl_size);
		}
		return false;
	}
};
