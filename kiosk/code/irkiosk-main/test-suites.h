#pragma once

#define TEST_SUITES \
	X(eeprom_hash) \


#define X(suite) extern void (*suite)();
TEST_SUITES
#undef X
