#pragma once

class Motor {
	protected:
	byte pin_dir1, pin_dir2, pin_pwm;

	public:
	Motor(byte pin_dir1, byte pin_dir2, byte pin_pwm) 
		: pin_dir1(pin_dir1), pin_dir2(pin_dir2), pin_pwm(pin_pwm) {
		pinMode(pin_dir1, OUTPUT);
		pinMode(pin_dir2, OUTPUT);
		pinMode(pin_pwm, OUTPUT);

		setSpeed(0);
	}

	void setSpeed(float speed) {
		if (speed == 0) {
			digitalWrite(pin_dir1, 0);
			digitalWrite(pin_dir2, 0);
			digitalWrite(pin_pwm, 0);
		} else if (speed > 0) {
			digitalWrite(pin_dir1, 1);
			digitalWrite(pin_dir2, 0);
			analogWrite(pin_pwm, 255 * speed);
		} else {
			digitalWrite(pin_dir1, 1);
			digitalWrite(pin_dir2, 1);
			analogWrite(pin_pwm, 255 * abs(speed));
		}
	}
};
