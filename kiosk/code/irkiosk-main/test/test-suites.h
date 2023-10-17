#pragma once

#define TEST_SUITES \
	X(test_HashSlot) \
	X(test_HashTable) \


#define X(suite) extern void (*suite)();
TEST_SUITES
#undef X
