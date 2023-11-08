#pragma once

#include "Rfid/HashTable.h"

// global state data structure
struct Model {
	struct Command {
		typedef enum {
			FORWARD, BACKWARD,
			LEFT, RIGHT, NONE
		} Action;
		Action action;
		float amount;
	};
	// commands sequence
	struct Command commands[4];

	// EEPROM hash table
	HashTable tbl;

	Model();
	void resetCommands();
};
