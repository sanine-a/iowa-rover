#include <Wire.h>
#include "PolledSwitch.h"
#include "pins.h"

#include "scheduler.h"
#include "signal.h"
#include "programmer/leds.h"


enum ButtonState {
	PRESSED, RELEASED
};
class Btn : public PolledSwitch, public Publisher<ButtonState> {
	public:
	Btn() : PolledSwitch(PROG_BTN1) {}
	void onLow() {
		publish(PRESSED);
		Serial.println("press!");
	}
	void onHigh() {
		publish(RELEASED);
		Serial.println("release!");
	}
} btn;


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


class Leds : public Programmer::Leds, public Subscriber<ButtonState> {
	public:
	Leds() : Subscriber(&btn) {}
	void on(ButtonState state) {
		if (state == PRESSED) {
			Serial.println("  subscriber got state PRESSED!");
		}
		else if (state == RELEASED) {
			Serial.println("  subscriber got state RELEASED!");
		}
	}
} leds;

Scheduler sch;

void setup() {
	Serial.begin(115200);
	leds.flashForward();
	sch.setTimeout([]{
		leds.clearForward();
		leds.flashBackward();
		sch.setTimeout([]{
			leds.clearBackward();
			leds.flashLeft();
			sch.setTimeout([]{
				leds.clearLeft();
				leds.flashRight();
				sch.setTimeout([]{
					leds.clearAll();
				}, 1000);
			}, 1000);
		}, 1000);
	}, 1000);
}

void loop() {
	sch.update();
	leds.update();
	btn.update();
}
