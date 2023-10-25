#pragma once

#include "Rfid/HashTable.h"

struct Model {
	struct Command {
		typedef enum {
			FORWARD, BACKWARD,
			LEFT, RIGHT, NONE
		} Action;
		Action action;
		float amount;
	};
	struct Command commands[4];

	HashTable tbl;

	Model();
	void resetCommands();
};
