#include "motor.h"
#include "Arduino.h"

void motor::setup(int pin_1, int pin_2, int pin_pwm)
{
	output_pin_1 = pin_1;
	output_pin_2 = pin_2;
	output_pin_pwm = pin_pwm;
	pinMode(output_pin_1, OUTPUT);
  pinMode(output_pin_2, OUTPUT);
  // pinMode(output_pin_pwm, OUTPUT);
  analogWrite(output_pin_pwm, 0);
  digitalWrite(output_pin_1, HIGH);
  digitalWrite(output_pin_2, LOW);
  input = new float;
}

void motor::operate(int val)
{
    *input = (float)val;
    this -> operate();
}

void motor::operate()
{
    digitalWrite(output_pin_1, input>=0? HIGH:LOW);
    digitalWrite(output_pin_2, input<0? HIGH:LOW);
    // digitalWrite(output_pin_1, HIGH);
    // digitalWrite(output_pin_2, LOW);
    analogWrite(output_pin_pwm, abs((int)input));
}

