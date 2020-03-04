// ALL Current sensor related code in main ctrl is commented out - 
// current sensor tested
// current sensor pid NOT tested,
#include "motor.h"
#include "sensor.h"
#include "config.h"
#include "control_4.h"

#include "TimerThree.h"
#include "TimerOne.h"

using namespace stimulator;
motor tor_motor;
motor vel_motor;

load_cell load_s;
encoder vel_enc;
void vel_update_a()
{
  vel_enc.measure_a();
}

void vel_update_b()
{
  vel_enc.measure_b();
}

/*
// encoder tor_enc;
void tor_update_a()
// to be used with input interrupts.
{
  tor_enc.measure_a();
}

void tor_update_b()
// to be used with input interrupts.
{
  tor_enc.measure_b();
}
*/

// current_sensor tor_curr;
current_sensor vel_curr;

// pid_controller tor_curr_pid;
// pid_controller vel_curr_pid;
pid_controller tor_load_s_pid;
pid_controller vel_enc_pid;

volatile bool updated = false;
volatile bool measured = false;


void update_updated(){
  updated = false;
}

void update_measured(){
  measured = false;
}

void raw_measures()
// to be used w/ timer interrupts.
{
  load_s.raw_measure();
  // tor_curr.raw_measure();
  vel_curr.raw_measure();
}

void update_values()
// to be used with timer interrupts.
{
  load_s.update_value();
  vel_enc.update_value();
  // tor_enc.update_value();
  // tor_curr.update_value();
  vel_curr.update_value();
}

void update_pid()
{
  tor_load_s_pid.update();
  vel_enc_pid.update();
  // tor_curr_pid.update();
  // vel_curr_pid.update();
}

void update_motors()
{
  // tor_motor.operate(tor_curr_pid.get_output());
  // vel_motor.operate(vel_enc_pid.get_output());
  // tor_motor.operate(tor_load_s_pid.get_output());
  vel_motor.operate();
  // tor_motor.operate(255);
//   vel_motor.operate(255);
}

void setup() {
  Serial.begin(9600);
  // load_s.setup(load_DAT, load_CLK);
  // put your setup code here, to run once:
  vel_motor.setup(vel_input_1, vel_input_2, vel_input_pwm, vel_enc_pid.get_p_output());
  tor_motor.setup(tor_input_1, tor_input_2, tor_input_pwm, tor_load_s_pid.get_p_output());
  
  load_s.setup(load_DAT, load_CLK, load_s_coef);
  // tor_enc.setup(tor_encoder_1, tor_encoder_2, encoder_ppr);
  vel_enc.setup(vel_encoder_1, vel_encoder_2, encoder_ppr);

  // tor_curr.setup(tor_curr_input);
  vel_curr.setup(vel_curr_input);

  
  tor_load_s_pid.setup(load_s.get_p_value(), &tor_load_s_pid_sp, tor_load_s_pid_coef);
  // tor_curr_pid.setup(tor_curr.get_p_value(), tor_load_s_pid.get_p_output(), tor_curr_pid_coef);
  vel_enc_pid.setup(vel_enc.get_p_value(), &vel_enc_pid_sp, vel_enc_pid_coef);
  // vel_curr_pid.setup(vel_curr.get_p_value(), vel_enc_pid.get_p_output(), vel_curr_pid_coef);

  vel_enc_pid.set_output_range(0.0, 255.0);
  tor_load_s_pid.activate();
  // tor_curr_pid.activate();
  vel_enc_pid.activate();
  // vel_curr_pid.activate();
  
  // attachInterrupt(digitalPinToInterrupt(tor_encoder_1), tor_update_a, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(tor_encoder_2), tor_update_b, CHANGE);

  attachInterrupt(digitalPinToInterrupt(vel_encoder_1), vel_update_a, CHANGE);
  attachInterrupt(digitalPinToInterrupt(vel_encoder_2), vel_update_b, CHANGE);

  Timer3.initialize(update_per);
  Timer3.attachInterrupt(update_updated);
  Timer1.initialize(measure_per);
  Timer1.attachInterrupt(update_measured);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!measured){
    raw_measures();
    measured = true;
  }
  if(!updated){
    // So if measured is false, values will be updated;
    update_values();
    update_pid();
    update_motors();
    updated = true;

    
    Serial.print("velocity pid output: ");
    Serial.println(vel_enc_pid.get_output());

    Serial.print("Velocity Encoder: ");
    Serial.println(vel_enc.get_value());

    // Serial.print("velocity current sensor: ");
    // Serial.println(vel_curr.get_value());
    
    /*
    Serial.print("Load Cell: ");
    Serial.println(load_s.get_value());
    Serial.println();

    Serial.print("torque pid output: ");
    Serial.println(tor_load_s_pid.get_output());
    
    
    Serial.println(vel_enc.get_value());
    */
  }

}
