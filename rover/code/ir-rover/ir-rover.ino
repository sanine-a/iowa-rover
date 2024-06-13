#include <stdarg.h>
#include <string.h>

#include "radioserial.h"
#include "motor.h"
#include "pins.h"


// radio constants
#define RADIO_FREQ 910.0

#define KIOSK1_ADDR 0x11
#define ROVER1_ADDR 0x12
#define KIOSK2_ADDR 0x21
#define ROVER2_ADDR 0x22
int rover_addr, kiosk_addr;


// movement constants
#define RAMP_TIME 300
#define MOVE_TIME 4000
#define MOVE_SPEED 1.0
#define TURN_TIME_MAX 3000
#define TURN_SPEED 0.75
#define PAUSE_TIME 500
#define POT_MAX ((2*1024)/3)


#define MOTOR_SPEED 1.0
Motor left_wheel (AIN1, AIN2, PWMA, MOTOR_SPEED);
Motor right_wheel(BIN1, BIN2, PWMB, MOTOR_SPEED);


// motor movements
float lerp(float x0, float x1, float t) {
  return ((1-t)*x0) + (t*x1);
}
void rampTo(float leftSpeed, float rightSpeed, unsigned long ms) {
  float left0 = left_wheel.getSpeed();
  float right0 = right_wheel.getSpeed();
  unsigned long startTime = millis();
  while (millis() < ms + startTime) {
    float t = ((float) millis() - startTime) / ms;
    left_wheel.setSpeed(lerp(left0, leftSpeed, t));
    right_wheel.setSpeed(lerp(right0, rightSpeed, t));
    delay(1);
  }
  left_wheel.setSpeed(leftSpeed);
  right_wheel.setSpeed(rightSpeed);
}
void drive(int direction, float amount) {
  rampTo(direction * MOVE_SPEED, direction * MOVE_SPEED, RAMP_TIME);
	delay((MOVE_TIME * amount) - RAMP_TIME);
}
double turn_adjust = 0.5;
void move_forward(float amount) { drive(1, amount); }
void move_backward(float amount) { drive(-1, amount); }

void turn(int direction, float amount) {
  rampTo(direction * TURN_SPEED, direction * -TURN_SPEED, RAMP_TIME);
	delay((turn_adjust * TURN_TIME_MAX * amount) - RAMP_TIME);
}
void turn_left(float amount) { turn(-1, amount); }
void turn_right(float amount) { turn(1, amount); }


// movement commands
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

	// helper function for command_t::print()
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

	// print the command's action and amount to serial
	void print() {
		Serial.print("[");
		Serial.print(action_str(action)); Serial.print(":"); Serial.print(amount);
		Serial.print("]");
	}
};


// radio communications
class Radio : public RadioSerial {
	public:
	Radio() : RadioSerial(RADIO_CS, RADIO_RST, RADIO_INT, RADIO_FREQ) {}

	// process incoming messages
	void onMessage(const char *key, const char *value) {
		Serial.print(" > "); Serial.print(key); Serial.print(":"); Serial.println(value);

		// if the key is "execute"...
		if (strcmp(key, "execute") == 0) {
			sendMessage("ready", 0); // tell the kiosk we are busy
			parse_commands(value);   // determine the command sequence we should run
			execute();               // run the commands
			sendMessage("ready", 1); // tell the kiosk we are no longer busy
			return;
		}

		// for all other messages, tell the kiosk we don't know what to do with it
		sendMessage("unknown-command", 1);
	}


	// determine command sequence from a sequence of characters and store in the commands[] array
	// ex: F1B2L3R4 -> (forward 0.125) (backward 0.25) (turn left 3.75) (turn right 0.5)
	bool parse_commands(const char *commands_buf) {
		// stack vars to hold our computation
		char actions[] = { 'N', 'N', 'N', 'N' };
		uint8_t amounts[] = { 4, 4, 4, 4 };

		// check if command_buf is the right length
		if (strlen(commands_buf) != 8) {
			Serial.print("WARNING: malformed command buffer: "); Serial.println(commands_buf);
			return false;
		}

		// iterate over the buffer
		for (int i=0; i<4; i++) {
			char buf[2]; buf[1] = 0;  // helper string for atoi() conversions
			const int idx = 2*i;
			actions[i] = commands_buf[idx]; // set the command character
			*buf = commands_buf[idx+1];
			amounts[i] = atoi(buf);         // set the command amount
		}

		// fill in commands[] array
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
			commands[i].amount = ((float)amounts[i])/8.0;  // convert integer in range [0-8] to float in range [0-1]
		}
		return true;
	}

	// print the current contents of the commands[] array
	void dump_commands() {
		for (int i=0; i<4; i++) {
			commands[i].print(); Serial.println();
		}
	}


	// protected:
	struct command_t commands[4];

	// execute a single command
	void execute_command(struct command_t cmd) {
		// do nothing for NONE actions
		if (cmd.action == command_t::action_t::NONE) {
			return;
		}

		// pause for actions with zero amount
		if (cmd.amount == 0) {
			// stop moving
			left_wheel.setSpeed(0);
			right_wheel.setSpeed(0);
			// pause
			delay(PAUSE_TIME);
			return;
		}

		// perform movements
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

	// sequentially execute the commands[] array
	void execute() {
		dump_commands();
		for (int i=0; i<4; i++) {
			Serial.print(i); Serial.print(" "); commands[i].print(); Serial.println();
			execute_command(commands[i]);
		}
		// need to stop at the end
		rampTo(0, 0, RAMP_TIME);
	}
} radio; // create our global radio object





void setup() {
	Serial.begin(115200);
	Serial.println();
	Serial.println("===== boot =====");
	Serial.println(sizeof(Radio));

	Serial.println("configuring radio...");
	radio.setup();
	Serial.println("done!");

	// choose address based on kiosk select switch
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

	// determine turn adjust from the trimpot
	long pot = analogRead(TRIMPOT);
	turn_adjust = ((float)pot)/POT_MAX;

	radio.parse_commands("F1R2L3R4");
	radio.execute();
}


void loop() {
	// poll for incoming transmissions
	radio.update();
}
