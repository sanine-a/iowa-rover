#include <lily-test.h>
#include "HashTable.h"
#include "Tag.h"


LILY_FILE_BEGIN(test_HashTable)


LILY_TEST("Store & retrieve RFID tags") {
	EEPROM.clear();
	RfidTag a(0x00, 0x00, 0x00, 0x00, 0x00);
	RfidTag b(0xaa, 0x80, 0x00, 0x00, 0x00);
	RfidTag c(0x12, 0x34, 0x56, 0x78, 0x9a);
	HashTable tbl(682);
	tbl.storeTag(a, 0x01);
	tbl.storeTag(b, 0x02);
	tbl.storeTag(c, 0x03);

	CHECK_EQ(tbl.findTag(a), 0x01, "%x");
	CHECK_EQ(tbl.findTag(b), 0x02, "%x");
	CHECK_EQ(tbl.findTag(c), 0x03, "%x");

	tbl.deleteTag(c);
	CHECK_EQ(tbl.findTag(a), 0x01, "%x");
	CHECK_EQ(tbl.findTag(b), 0x02, "%x");
	CHECK_EQ(tbl.findTag(c), CATEGORY_UNKNOWN, "%x");

	tbl.deleteTag(a);
	CHECK_EQ(tbl.findTag(a), CATEGORY_UNKNOWN, "%x");
	CHECK_EQ(tbl.findTag(b), 0x02, "%x");
	CHECK_EQ(tbl.findTag(c), CATEGORY_UNKNOWN, "%x");
	
	tbl.storeTag(a, 0x04);
	CHECK_EQ(tbl.findTag(a), 0x04, "%x");
	CHECK_EQ(tbl.findTag(b), 0x02, "%x");
	CHECK_EQ(tbl.findTag(c), CATEGORY_UNKNOWN, "%x");
}
#include LILY_PUSH_TEST()


#define LILY_FILE_END
#include LILY_REGISTER_TESTS()
