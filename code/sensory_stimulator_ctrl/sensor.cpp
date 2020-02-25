#include "sensor.h"
#include "Arduino.h"
#include "HX711.h" 
#include "config.h"
// Please refer to: https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide


template <class C>
sensor<C>::sensor(){};

template <class C>
void sensor<C>::setup(int pin)
{
  input_pin = pin;
  pinMode(input_pin, INPUT);
}

template <class C>
void sensor<C>::raw_measure()
// to be called at a rate higher than the PID update steps, 
// measurements stored in an array, with average calculated for each PID update step
{
  raw_val_sum += analogRead(input_pin);
  ++ cur_raw_count;
}

template <class C>
void sensor<C>::measure()
// calculates the average of measurements instead of making real measurements
{
  cur_val = (float)raw_val_sum / (float)cur_raw_count; // cur_val serves as an intermediate step, can be used for further processing.
  cur_out = cur_val; 
  cur_raw_count = 0;  
  raw_val_sum = 0;
}

template <class C>
void sensor<C>::update_value(){
  measure();
}

template <class C>
float sensor<C>::get_value()
// Your typical and lonely getter method
{
  return cur_out;
}

template <class C>
float* sensor<C>::get_p_value()
{
  return & cur_out;
}

load_cell::load_cell() : sensor<float>(){}

void load_cell::setup(int n_DOUT, int n_CLK, int n_coef)
{
  CLK = n_CLK;
  input_pin = n_DOUT;
  coef = n_coef;
  pinMode(CLK, INPUT);
  pinMode(input_pin, INPUT);
  scale.begin(input_pin, CLK);
  scale.set_scale(coef);
  
  scale.tare();
}

void load_cell::raw_measure(){
  if(1){
    raw_val_sum += scale.get_units(5);
    ++ cur_raw_count;  
  }
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
  circ =  (float)stimulator::drum_diameter * 3.1415926;
}

void encoder::setup(int pin_a, int pulse_per_rotation)
{
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
  cur_rps = ((float)cur_rotat_count + (float)cur_pulse_count/(float)ppr) / (stimulator::update_per) * 1000000;
  cur_out = circ * cur_rps;
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
  return cur_out;
}

float * encoder::get_p_value()
{
  return & cur_out; 
}

float encoder::get_rpm()
{
  float rpm = cur_rps * 60;
  return rpm;
}

// Sort out the template issues:
template class sensor<int>;
template class sensor<float>;
