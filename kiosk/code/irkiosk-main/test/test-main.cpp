#define LILY_IMPLEMENTATION
#include "lily-test.h"
#include "test-suites.h"

int main() {
	lily_begin();

	// run test suites
	#define X(suite) suite();
	TEST_SUITES
	#undef X

	lily_finish();
	return 0;
}
