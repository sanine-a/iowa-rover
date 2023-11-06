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
	delay(8000 * amount);
}
void move_forward(float amount) { drive(1, amount); }
void move_backward(float amount) { drive(-1, amount); }
void turn(int direction, float amount) {
	left_wheel.setSpeed(direction * 0.5);
	right_wheel.setSpeed(direction * -0.5);
	delay(3850 * amount);
}
void turn_left(float amount) { turn(-1, amount); }
void turn_right(float amount) { turn(1, amount); }
void pause() { 
	left_wheel.setSpeed(0);
	right_wheel.setSpeed(0);
	delay(500);
}


struct command_t {
	typedef enum {
		MOVE_FORWARD,
		MOVE_BACKWARD,
		TURN_LEFT,
		TURN_RIGHT,
		NONE,
	} action_t;
	action_t action;
	float amount;

	static char action_str(action_t a) {
		switch(a) {
		case action_t::MOVE_FORWARD: return 'F';
		case action_t::MOVE_BACKWARD: return 'B';
		case action_t::TURN_LEFT: return 'L';
		case action_t::TURN_RIGHT: return 'R';
		case action_t::NONE: return 'N';
		default: return '?';
		}
	}

	void print() {
		Serial.print("[");
		Serial.print(action_str(action)); Serial.print(":"); Serial.print(amount);
		Serial.print("]");
	}
};


class Radio : public RadioSerial {
	public:
	Radio() : RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, RADIO_FREQ) {}
	void onMessage(const char *key, const char *value) {
		Serial.print(" > "); Serial.print(key); Serial.print(":"); Serial.println(value);
		if (strcmp(key, "execute") == 0) {
		char buf[16];
		snprintf(buf, sizeof(buf), "%p", commands);
		Serial.println(buf);

			sendMessage("ready", 0);
			parse_commands(value);
		Serial.println("c:");
		snprintf(buf, sizeof(buf), "%p", commands);
		Serial.println(buf);

			execute();
			sendMessage("ready", 1);
			return;
		}

		sendMessage("unknown-command", 1);
	}


	bool parse_commands(const char *commands_buf) {
		Serial.println(commands_buf);
		char actions[] = { 'N', 'N', 'N', 'N' };
		uint8_t amounts[] = { 4, 4, 4, 4 };
		if (strlen(commands_buf) != 8) {
			Serial.print("WARNING: malformed command buffer: "); Serial.println(commands_buf);
			return false;
		}
		for (int i=0; i<4; i++) {
			char buf[2]; buf[1] = 0;
			const int idx = 2*i;
			actions[i] = commands_buf[idx];
			*buf = commands_buf[idx+1];
			Serial.print(idx+1); Serial.print(" "); Serial.println(buf);
			amounts[i] = atoi(buf);
		}

		for (int i=0; i<4; i++) {
			Serial.print(actions[i]); Serial.print(" "); Serial.println(amounts[i]);
			switch(actions[i]) {
				case 'F': commands[i].action = command_t::action_t::MOVE_FORWARD; break;
				case 'B': commands[i].action = command_t::action_t::MOVE_BACKWARD; break;
				case 'L': commands[i].action = command_t::action_t::TURN_LEFT; break;
				case 'R': commands[i].action = command_t::action_t::TURN_RIGHT; break;
				case 'N': commands[i].action = command_t::action_t::NONE; break;
				default: commands[i].action = command_t::action_t::NONE; break;
			}
			commands[i].amount = ((float)amounts[i])/8.0;
		}
		dump_commands();
		char buf[16];
		snprintf(buf, sizeof(buf), "%p", commands);
		Serial.println(buf);
		return true;
	}

	void dump_commands() {
		for (int i=0; i<4; i++) {
			commands[i].print(); Serial.println();
		}
	}


	protected:
	struct command_t commands[4];

	void execute_command(struct command_t cmd) {
		if (cmd.amount == 0) {
			left_wheel.setSpeed(0);
			right_wheel.setSpeed(0);
			delay(500);
			return;
		}

		switch(cmd.action) {
		case command_t::action_t::MOVE_FORWARD:
			move_forward(cmd.amount);
			break;
		case command_t::action_t::MOVE_BACKWARD:
			move_backward(cmd.amount);
			break;
		case command_t::action_t::TURN_LEFT:
			turn_left(cmd.amount);
			break;
		case command_t::action_t::TURN_RIGHT:
			turn_right(cmd.amount);
			break;
		default:
			Serial.print("WARNING: unknown action: "); Serial.println(cmd.action);
			break;
		}
	}

	void execute() {
		dump_commands();
		for (int i=0; i<4; i++) {
			Serial.print(i); Serial.print(" "); commands[i].print(); Serial.println();
			execute_command(commands[i]);
			delay(500);
		}
		left_wheel.setSpeed(0);
		right_wheel.setSpeed(0);
	}
} radio;





void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.println("===== boot =====");
	Serial.println(sizeof(Radio));

	radio.parse_commands("B1F1B1F1");

	Serial.println("configuring radio...");
	radio.setup();
	Serial.println("done!"); delay(1000);

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
	Serial.print("using rover address 0x"); Serial.println(rover_addr, HEX);
	Serial.print("expecting transmissions from kiosk address 0x"); Serial.println(kiosk_addr, HEX);
}


void loop() {
	radio.update();
}
