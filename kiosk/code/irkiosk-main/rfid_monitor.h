#pragma once

#include "model.h"
#include "scheduler.h"
#include "signal.h"
#include "rfid_reader.h"
#include "Rfid/HashTable.h"

// class to print RFID events to serial
class RfidMonitor : public Subscriber<RfidEvent> {
	public:
	RfidMonitor(HashTable& tbl, Rfid& rfid);
	void on(RfidEvent e);
	private:
	HashTable& tbl;
};


// class to periodically reboot the RFID modules to check if tags have been removed
class RfidPoller : public Subscriber<RfidEvent> {
	public:
	RfidPoller(Model& model, Scheduler& sch, Rfid& rfid);
	void on(RfidEvent e);
	private:
	Model& model;
	Scheduler& sch;
	Rfid& rfid;
	bool polling;
	bool reset[4];
	unsigned int resetTimeout[4];
	void startPoll();
	void endPoll();
};
