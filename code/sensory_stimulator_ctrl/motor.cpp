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
  p_input = NULL;
}

void motor::setup(int pin_1, int pin_2, int pin_pwm, float* n_p_input){
  this -> setup(pin_1, pin_2, pin_pwm);
  p_input = n_p_input;
}

void motor::operate()
{
    if(p_input != NULL) this -> operate(*p_input);
    else operate(input);
}

void motor::operate(float val)
{   
    digitalWrite(output_pin_1, val >= 0? HIGH:LOW);
    digitalWrite(output_pin_2, val < 0? HIGH:LOW);
    // digitalWrite(output_pin_1, HIGH);
    // digitalWrite(output_pin_2, LOW);
    analogWrite(output_pin_pwm, abs((int)val));
}

float* motor::get_ext_p_input()
{
  return p_input;
}

float* motor::get_p_input()
{
  return &input;
}
