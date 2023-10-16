#include <lily-test.h>
#include "HashSlot.h"
#include "Tag.h"

LILY_FILE_BEGIN(test_HashSlot)

LILY_TEST("Get slot address") {
	struct HashSlot hs = { 0 };
	CHECK_EQ(hs.address(), 0, "%d");
	hs.slot_index = 20;
	CHECK_EQ(hs.address(), 120, "%d");
	hs.slot_index = 5;
	CHECK_EQ(hs.address(), 30, "%d");
}
#include LILY_PUSH_TEST()


LILY_TEST("Check if a slot is occupied") {
	struct HashSlot hs = { 10 };
	EEPROM.clear();
	
	CHECK_EQ(hs.isOccupied(false), false, "%d");
	CHECK_EQ(hs.isOccupied(true),  false, "%d");

	EEPROM.write(60, 0x7f); // occupied but deleted
	CHECK_EQ(hs.isOccupied(false), false, "%d");
	CHECK_EQ(hs.isOccupied(true),  true,  "%d");

	EEPROM.write(60, 0x3f); // occupied and not deleted
	CHECK_EQ(hs.isOccupied(false), true, "%d");
	CHECK_EQ(hs.isOccupied(true),  true,  "%d");
}
#include LILY_PUSH_TEST()


LILY_TEST("Matching tags") {
	RfidTag tag(0, 1, 2, 3, 4);
	EEPROM.clear();
	EEPROM.write(31, 0);
	EEPROM.write(32, 1);
	EEPROM.write(33, 2);
	EEPROM.write(34, 3);
	EEPROM.write(35, 4);
	struct HashSlot hs = { 5 };
	
	CHECK_EQ(hs.matchesTag(tag), true, "%d");
	EEPROM.write(35, 3);
	CHECK_EQ(hs.matchesTag(tag), false, "%d");
}
#include LILY_PUSH_TEST()


LILY_TEST("Set/unset the occupied & deleted bits") {
	struct HashSlot hs { 4 };
	EEPROM.clear();
	EEPROM.write(24, 0x33);

	hs.setOccupied(false);
	CHECK_EQ(EEPROM.read(24) & FLAG_UNOCCUPIED, FLAG_UNOCCUPIED, "%d");
	CHECK_EQ(EEPROM.read(24) & FLAG_CATEGORY, 0x33, "%d");
	hs.setOccupied(true);
	CHECK_EQ(EEPROM.read(24) & FLAG_UNOCCUPIED, 0, "%d");
	CHECK_EQ(EEPROM.read(24) & FLAG_CATEGORY, 0x33, "%d");

	hs.setDeleted(false);
	CHECK_EQ(EEPROM.read(24) & FLAG_DELETED, 0, "%d");
	CHECK_EQ(EEPROM.read(24) & FLAG_CATEGORY, 0x33, "%d");
	hs.setDeleted(true);
	CHECK_EQ(EEPROM.read(24) & FLAG_DELETED, FLAG_DELETED, "%d");
	CHECK_EQ(EEPROM.read(24) & FLAG_CATEGORY, 0x33, "%d");
}
#include LILY_PUSH_TEST()


LILY_TEST("Get/set the category bits") {
	EEPROM.clear();
	struct HashSlot hs = { 20 };
	CHECK_EQ(hs.getCategory(), 0x3f, "%x");
	EEPROM.write(120, 0xab);
	CHECK_EQ(hs.getCategory(), 0x2b, "%x");

	EEPROM.write(120, 0x00);
	hs.setCategory(0xff);
	CHECK_EQ(EEPROM.read(120), 0x3f, "%x");
}
#include LILY_PUSH_TEST()


#define LILY_FILE_END
#include LILY_REGISTER_TESTS()
