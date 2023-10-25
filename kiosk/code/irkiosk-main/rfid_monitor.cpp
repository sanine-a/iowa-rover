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
