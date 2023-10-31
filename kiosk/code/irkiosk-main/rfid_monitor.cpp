#include "rfid_monitor.h"

RfidMonitor::RfidMonitor(HashTable& tbl, Rfid& rfid) : tbl(tbl), Subscriber(&rfid) {}
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
	sch.setInterval([this]{ startPoll(); }, 1000);
}


void RfidPoller::startPoll() {
	Serial.println("  poll> start");
	rfid.reset();
	memset(reset, true, sizeof(reset));
	polling = true;
	sch.setTimeout([this]{ endPoll(); }, 500);
}
void RfidPoller::endPoll() {
	polling = false;
	for (int i=0; i<4; i++) {
		if (reset[i]) {
			model.commands[i].action = Model::Command::Action::NONE;
		}
	}
	Serial.println("  poll> end");
}


void RfidPoller::on(RfidEvent e) {
	if (!polling) { return; }
	int idx = e.sourceAddr - 0x71;
	if (idx < 0) { return; }
	Serial.print("  poll> read on "); Serial.println(idx);
	reset[idx] = false;
}
