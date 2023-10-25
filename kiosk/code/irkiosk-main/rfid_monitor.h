#pragma once

#include "signal.h"
#include "rfid_reader.h"
#include "Rfid/HashTable.h"

class RfidMonitor : public Subscriber<RfidEvent> {
	public:
	RfidMonitor(HashTable& tbl, Rfid& rfid);
	void on(RfidEvent e);
	private:
	HashTable& tbl;
};
