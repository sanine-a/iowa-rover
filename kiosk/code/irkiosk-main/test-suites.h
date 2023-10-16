#pragma once

#define TEST_SUITES \
	X(test_HashSlot) \


#define X(suite) extern void (*suite)();
TEST_SUITES
#undef X
