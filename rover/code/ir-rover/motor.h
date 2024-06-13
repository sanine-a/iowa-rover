#pragma once

// H-bridge motor controller
class Motor {
	protected:
	byte pin_dir1, pin_dir2, pin_pwm;
	float alpha;
	float speed;

	public:
	// constructor
	//   pin_dir1: pin to use for direction control
	//   pin_dir2: pin to use for direction control
	//   pin_pwm:  pin to use for speed control
	//   speed:    maximum speed of the motor
	Motor(byte pin_dir1, byte pin_dir2, byte pin_pwm, float speed) 
		: pin_dir1(pin_dir1), pin_dir2(pin_dir2), pin_pwm(pin_pwm), alpha(speed) {
		pinMode(pin_dir1, OUTPUT);
		pinMode(pin_dir2, OUTPUT);
		pinMode(pin_pwm, OUTPUT);

		setSpeed(0);
	}

	// set the motor speed
	// speed is scaled by the motor's maximum speed
	void setSpeed(float speed) {
		this->speed = alpha * speed; // scale
		if (speed == 0) {
			// stop
			digitalWrite(pin_dir1, 0);
			digitalWrite(pin_dir2, 0);
			digitalWrite(pin_pwm, 0);
		} else if (speed > 0) {
			// run "forward"
			digitalWrite(pin_dir1, 1);
			digitalWrite(pin_dir2, 0);
			analogWrite(pin_pwm, 255.0 * this->speed);
		} else {
			// run "backward"
			digitalWrite(pin_dir1, 0);
			digitalWrite(pin_dir2, 1);
			analogWrite(pin_pwm, 255.0 * (-1.0 * this->speed));
		}
	}

	float getSpeed() { return speed; }
};
