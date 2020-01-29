#include "i.h"
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
}

void motor::setup(int pin_1, int pin_2, int pin_pwm, float * n_input)
{
  setup(pin_1, pin_2, pin_pwm);
  input = n_input;
  input_set = true;
}

void motor::operate()
{
  if(input_set) operate((int)(*input));
}

void motor::operate(int val)
{
    digitalWrite(output_pin_1, val>=0? HIGH:LOW);
    digitalWrite(output_pin_2, val<0? HIGH:LOW);
    // digitalWrite(output_pin_1, HIGH);
    // digitalWrite(output_pin_2, LOW);
    analogWrite(output_pin_pwm, abs(val));
}
