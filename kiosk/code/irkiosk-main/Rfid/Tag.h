#pragma once

#include <stdio.h>
#include <string.h>

struct RfidTag {
	// our internal array c:
	unsigned char tagData[5];

	// empty constructor, leaves the array uninitialized
	RfidTag() {}
	// constructor to initialize array values
	RfidTag(unsigned char d0, unsigned char d1, unsigned char d2, unsigned char d3, unsigned char d4) {
		tagData[0] = d0;
		tagData[1] = d1;
		tagData[2] = d2;
		tagData[3] = d3;
		tagData[4] = d4;
	}

	// handy content equality operator
	friend bool operator==(RfidTag& lhs, RfidTag& rhs) {
		return lhs[0] == rhs[0] &&
		lhs[1] == rhs[1] &&
		lhs[2] == rhs[2] &&
		lhs[3] == rhs[3] &&
		lhs[4] == rhs[4];
	}
	// handy content inequality operator
	friend bool operator!=(RfidTag& lhs, RfidTag& rhs) { return !(lhs == rhs); }
	// index operator for the underlying array
	unsigned char& operator[](int index) { return tagData[index]; }
	
	// build a string representation of the tag
	// please be nice and make sure that your char array is at least 16 characters long!
	void toString(char *str, size_t sz) {
		snprintf(str, sz,
			 "%02x %02x %02x %02x %02x",
			 tagData[0], tagData[1], tagData[2], tagData[3], tagData[4]);
	}

	// compute the checksum of the array data
	unsigned char checksum() {
		unsigned char check = 0x00;
		for (int i=0; i<5; i++)
		check ^= tagData[i];
		return check;
	}

	// check if the array is "empty" (all values are 0xff)
	bool isEmpty() {
	    if (tagData[0] != 255)
			return false;
	    if (tagData[1] != 255)
			return false;
	    if (tagData[2] != 255)
			return false;
	    if (tagData[3] != 255)
			return false;
	    if (tagData[4] != 255)
			return false;
	    return true;
	}	

	// hash the tag for use with a hash table
	int hash(unsigned int tbl_size) {
		// split into segments of 10 bits each
		int a = (tagData[0] << 2) | (tagData[1] >> 6);
		int b = (tagData[1] << 4) | (tagData[2] >> 4);
		int c = (tagData[2] << 6) | (tagData[3] >> 2);
		int d = (tagData[3] << 8) | (tagData[4] >> 0);

		// sum segments, mod table size
		return (a + b + c + d) % tbl_size;
	}
};
