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


#define MOTOR_SPEED 1.0
Motor left_wheel (AIN1, AIN2, PWMA, MOTOR_SPEED);
Motor right_wheel(BIN1, BIN2, PWMB, MOTOR_SPEED);


// motor movements
void drive(int direction, float amount) {
	left_wheel.setSpeed(direction * 1.0);
	right_wheel.setSpeed(direction * 1.0);
	delay(2000 * amount);
}
void move_forward(float amount) { drive(1, amount); }
void move_backward(float amount) { drive(-1, amount); }
void turn(int direction, float amount) {
	left_wheel.setSpeed(direction * 1.0);
	right_wheel.setSpeed(direction * -1.0);
	delay(2000 * amount);
}
void turn_left(float amount) { turn(-1, amount); }
void turn_right(float amount) { turn(1, amount); }


struct command_t {
	typedef enum {
		MOVE_FORWARD,
		MOVE_BACKWARD,
		TURN_LEFT,
		TURN_RIGHT,
	} action_t;
	action_t action;
	float amount;
};


class Radio : public RadioSerial {
	public:
	Radio() : RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, RADIO_FREQ), command_idx(-1) {}
	void onMessage(const char *key, const char *value) {
		Serial.print(" > "); Serial.print(key); Serial.print(":"); Serial.println(value);
		if (strcmp(key, "command") == 0) {
			command_idx = atoi(value);
			// ensure indices stay in the valid range
			if (command_idx >= sizeof(commands)/sizeof(struct command_t)) {
				command_idx = -1;
				sendMessage("invalid-command-index", 1);
			}
			return;
		}

		if (strcmp(key, "action") == 0) {
			if (command_idx < 0) {
				sendMessage("invalid-command-index", 1);
			} else {
				commands[command_idx].action = atoi(value);
			}
			return;
		}

		if (strcmp(key, "amount") == 0) {
			if (command_idx < 0) {
				sendMessage("invalid-command-index", 1);
			} else {
				commands[command_idx].amount = strtod(value, nullptr);
			}
			return;
		}

		if (strcmp(key, "execute") == 0) {
			sendMessage("ready", 0);
			execute();
			command_idx = -1;
			sendMessage("ready", 1);
			return;
		}

		sendMessage("unknown-command", 1);
	}

	protected:
	int command_idx;
	struct command_t commands[4];

	void execute_command(struct command_t cmd) {
		switch(cmd.action) {
		case command_t::action_t::MOVE_FORWARD:
			Serial.print("forward "); Serial.println(cmd.amount);
			move_forward(cmd.amount);
			break;
		case command_t::action_t::MOVE_BACKWARD:
			Serial.print("backward "); Serial.println(cmd.amount);
			move_backward(cmd.amount);
			break;
		case command_t::action_t::TURN_LEFT:
			Serial.print("left "); Serial.println(cmd.amount);
			turn_left(cmd.amount);
			break;
		case command_t::action_t::TURN_RIGHT:
			Serial.print("right "); Serial.println(cmd.amount);
			turn_right(cmd.amount);
			break;
		default:
			Serial.println("WARNING: unknown action!");
			break;
		}
	}

	void execute() {
		for (int i=0; i<command_idx+1; i++) {
			execute_command(commands[i]);
		}
		left_wheel.setSpeed(0);
		right_wheel.setSpeed(0);
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
