#include <Wire.h>

void check_addr(uint8_t addr) {
	Wire.requestFrom(addr, 6);
	Serial.print("0x");
	Serial.print(addr, HEX);
	Serial.print(": ");
	Serial.println(Wire.available());
	while (Wire.available()) {
		Serial.println(Wire.read(), HEX);
	}

}

void setup() {
	Serial.begin(115200);

	Wire.begin();

	check_addr(0x70);
	check_addr(0x71);
	check_addr(0x72);
	check_addr(0x73);
	check_addr(0x74);
}

void loop() {}
