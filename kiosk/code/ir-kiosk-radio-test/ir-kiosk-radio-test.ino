#include "radioserial.h"
#include "schedule.h"
#include "pins.h"


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


Scheduler<8> sch;


void setup() {
	Serial.begin(115200);
	radio.setup();
  radio.set_addrs(0x22, 0x21);
	sch.setInterval([]{
		Serial.println("tx < ping:1");
		radio.sendMessage("ping", 1);
	}, 2000);
}


void loop() {
	radio.update();
	sch.update();
}
