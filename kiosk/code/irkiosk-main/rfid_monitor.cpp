#include "rfid_monitor.h"

RfidMonitor::RfidMonitor(HashTable& tbl, Rfid& rfid) : tbl(tbl), Subscriber(&rfid) {}

// on RFID event, print event to serial
void RfidMonitor::on(RfidEvent e) {
	Serial.println("== RFID READ ==");
	Serial.print("  reader: 0x"); Serial.println(e.sourceAddr, HEX);
	char tagStr[16]; e.tag.toString(tagStr, sizeof(tagStr));
	Serial.print("  tag: ["); Serial.print(tagStr); Serial.println("]");
	Serial.print("  category: 0x"); Serial.println(tbl.findTag(e.tag), HEX);
	Serial.println();
}


// --===== RfidPoller =====--


RfidPoller::RfidPoller(Model& model, Scheduler& sch, Rfid& rfid)
: model(model), sch(sch), rfid(rfid), Subscriber(&rfid) {
	memset(resetTimeout, 0, sizeof(resetTimeout));
	sch.setInterval([this]{ startPoll(); }, 1000);
}


// reset the RFID modules and wait for responses
void RfidPoller::startPoll() {
	rfid.reset();
	memset(reset, true, sizeof(reset));
	polling = true;
	sch.setTimeout([this]{ endPoll(); }, 500);
}
// done waiting for responses, clear slots without tag reads
void RfidPoller::endPoll() {
	polling = false;
	for (int i=0; i<4; i++) {
		if (reset[i]) {
			model.commands[i].action = Model::Command::Action::NONE;
			// sometimes reset reads fail
			// so we set a 2000 ms timeout to set the command amount back to 0, and clear it if
			// we get a read before the timeout completes
			// this prevents the amount semi-randomly resetting to 0.5 even though the tag was
			// never removed.
			resetTimeout[i] = sch.setTimeout([this, i]{
				model.commands[i].amount = 0.5;
			}, 2000);
		}
	}
}


// respond to RFID events
void RfidPoller::on(RfidEvent e) {
	int idx = e.sourceAddr - 0x71;
	if (idx < 0) { return; }
	if (idx > 3) { return; }

	// clear amount reset timeouts
	sch.clearTimeout(resetTimeout[idx]);

	if (polling) { 
		reset[idx] = false;
	}
}
