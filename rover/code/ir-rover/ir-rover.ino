#include <stdarg.h>
#include <string.h>

#include "radioserial.h"
#include "motor.h"
#include "pins.h"

#define RADIO_FREQ 910.0

#define KIOSK1_ADDR 0x11
#define ROVER1_ADDR 0x12
#define KIOSK2_ADDR 0x21
#define ROVER2_ADDR 0x22
int rover_addr, kiosk_addr;


Motor left_wheel (AIN1, AIN2, PWMA);
Motor right_wheel(BIN1, BIN2, PWMB);



class Radio : public RadioSerial {
	public:
	Radio() : RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, RADIO_FREQ) {}
	void onMessage(const char *key, const char *value) {
		if (strcmp(key, "left") == 0) {
			left_wheel.setSpeed(strtod(value, nullptr));
			return;
		}

		if (strcmp(key, "right") == 0) {
			right_wheel.setSpeed(strtod(value, nullptr));
			return;
		}

		if (strcmp(key, "ping") == 0) {
			sendMessage("pong", 1);
			return;
		}

		sendMessage("unknown-command", 1);
	}
} radio;



void serial_printf(const char *fmt, ...) {
	static char buf[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 256, fmt, args);
	va_end(args);
	Serial.print(buf);
}


void setup() {
	Serial.begin(115200);

	Serial.println("configuring radio...");
	radio.setup();
	Serial.println("done!");

	// choose address
	pinMode(KIOSK_SELECT, INPUT_PULLUP);
	if (!digitalRead(KIOSK_SELECT)) {
		rover_addr = ROVER1_ADDR;
		kiosk_addr = KIOSK1_ADDR;
	} else {
		rover_addr = ROVER2_ADDR;
		kiosk_addr = KIOSK2_ADDR;
	}
	radio.set_addrs(rover_addr, kiosk_addr);
	serial_printf("using rover address 0x%x\n", rover_addr);
	serial_printf("expecting transmissions from kiosk address 0x%x\n", kiosk_addr);
}


void loop() {
	radio.update();
}
