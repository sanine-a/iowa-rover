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


#define LILY_FILE_END
#include LILY_REGISTER_TESTS()
