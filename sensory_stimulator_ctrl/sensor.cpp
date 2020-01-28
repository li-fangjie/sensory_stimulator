#include "sensor.h"
#include "Arduino.h"
#include "HX711.h" 
#include "config.h"
// Please refer to: https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide

#define calibration_factor -7050.0 // Calibration factor, which we need to acquire with from calibration tests.

sensor::sensor(){};

void sensor::setup(int pin)
{
  input_pin = pin;
  pinMode(input_pin, INPUT);
}

void sensor::raw_measure()
// to be called at a rate higher than the PID update steps, 
// measurements stored in an array, with average calculated for each update step
{
  raw_val_s[cur_raw_count] = analogRead(input_pin);
  ++cur_raw_count;
}

void sensor::measure()
// Only calculates the average of measurements instead of making real measurements
{
  float sum;
  for(int i=0;i<cur_raw_count;++i){
  sum += raw_val_s[cur_raw_count];
  }
  cur_val = (float)sum / (float)cur_raw_count;
  cur_out = cur_out;
  cur_raw_count = 0;  
}

void sensor::update_value(){
  measure();
}

float sensor::get_value()
// Your typical and lonely getter method
{
  return cur_out;
}

float* sensor::get_p_value()
{
  return & cur_out;
}

load_cell::load_cell() : sensor(){}

void load_cell::setup(int n_DOUT, int n_CLK)
{
  CLK = n_CLK;
  input_pin = n_DOUT;
  pinMode(CLK, INPUT);
  pinMode(input_pin, INPUT);
  scale.begin(input_pin, CLK);
  scale.set_scale(calibration_factor);
  
  scale.tare();
}

void load_cell::measure(){
  cur_val = scale.get_value(5);
}

void load_cell::update_value(){
  measure();
}

current_sensor::current_sensor() : sensor(){}

void current_sensor::update_value()
{
  measure();
  cur_out = ((float)cur_val / (float)1024 * 5000 - (float)offset_mv) / sensitivity_mv;
}

encoder::encoder(){}
void encoder::setup(int pin_a, int pin_b, int pulse_per_rotation)
{
  input_pin_a = pin_a;
  input_pin_b = pin_b;
  pinMode(input_pin_a, INPUT_PULLUP);
  pinMode(input_pin_a, INPUT_PULLUP);
  ppr = pulse_per_rotation * 4; // With two interrupts, each capable of counting rising+falling edges, the precision can be quadrupled.
  c =  (float)stimulator::drum_diameter * 3.1415926;
}

void encoder::setup(int pin_a, int pulse_per_rotation)
{
  time_period = (float)stimulator::update_per / 1000000;
  input_pin_a = pin_a;
  ppr = pulse_per_rotation * 2; // same as above, but with 1 inerrupt, precision is doubled;
}

void encoder::measure_a() 
// update_pulse_count for A phase
// This method handles interrupts from encoders
// sorts out pulse count.
{
  cur_pulse_count += digitalRead(input_pin_a) == digitalRead(input_pin_b) ? -1 : 1;
  if (abs(cur_pulse_count) >= ppr){
    cur_rotat_count += abs(cur_pulse_count)/cur_pulse_count;
    cur_pulse_count = 0;
  }
}

void encoder::measure_b()
// update_pulse_count for B phase
// This method handles interrupts from encoders
// sorts out pulse count.
{
  cur_pulse_count += digitalRead(input_pin_a) != digitalRead(input_pin_b) ? -1 : 1;
  if (abs(cur_pulse_count) >= ppr){
    cur_rotat_count += abs(cur_pulse_count)/cur_pulse_count;
    cur_pulse_count = 0;
  }
}

void encoder::update_velocity()
// This method handles interrupts from timer
// calculates linear velocity.
{
  // The distance travelled from last calculation, in mm.
  cur_rps = ((float)cur_rotat_count + (float)cur_pulse_count/(float)ppr) / time_period;
  cur_val = c * cur_rps;
  // Serial.print("cur pulse count:");
  // Serial.println(cur_pulse_count);
  cur_rotat_count = 0; // resets rotation count for next speed measurements.
  cur_pulse_count = 0;
}

void encoder::update_value()
{
  update_velocity();  
}

float encoder::get_value()
// Your typical and lonely getter method
{
  return cur_val;
}

float * encoder::get_p_value()
{
  return & cur_val; 
}

float encoder::get_rpm()
{
  float rpm = cur_rps * 60;
  return rpm;
}
