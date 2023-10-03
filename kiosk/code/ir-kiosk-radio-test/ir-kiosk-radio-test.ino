#include "radioserial.h"
#include "schedule.h"
#include "pins.h"

#include "SerialController.hpp"


class Radio : public RadioSerial {
	public:
	Radio() : RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, 910.0) {}
	void onMessage(const char *key, const char *value) {
		Serial.print("    RX > ");
		Serial.print(key);
		Serial.print(":");
		Serial.println(value);
	}
} radio;


SerialController serial;

void on_serial(char *key, char *value) {
	Serial.print("tx < "); Serial.print(key); Serial.print(":"); Serial.println(value);
	radio.sendMessage(key, value);
}


Scheduler<8> sch;


void setup() {
	serial.setup(115200, on_serial);
	radio.setup();
	radio.set_addrs(0x21, 0x22);
}


void loop() {
	radio.update();
	//sch.update();
	serial.update();
}
