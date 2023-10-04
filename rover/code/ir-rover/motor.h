#pragma once

class Motor {
	protected:
	byte pin_dir1, pin_dir2, pin_pwm;
	float alpha;

	public:
	Motor(byte pin_dir1, byte pin_dir2, byte pin_pwm, float speed) 
		: pin_dir1(pin_dir1), pin_dir2(pin_dir2), pin_pwm(pin_pwm), alpha(speed) {
		pinMode(pin_dir1, OUTPUT);
		pinMode(pin_dir2, OUTPUT);
		pinMode(pin_pwm, OUTPUT);

		setSpeed(0);
	}

	void setSpeed(float speed) {
		float s = alpha * speed;
		if (speed == 0) {
			digitalWrite(pin_dir1, 0);
			digitalWrite(pin_dir2, 0);
			digitalWrite(pin_pwm, 0);
		} else if (speed > 0) {
			digitalWrite(pin_dir1, 1);
			digitalWrite(pin_dir2, 0);
			analogWrite(pin_pwm, 255.0 * s);
		} else {
			s *= -1;
			digitalWrite(pin_dir1, 0);
			digitalWrite(pin_dir2, 1);
			analogWrite(pin_pwm, 255.0 * s);
		}
	}
};
