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
// The setpoints for load cell and encoder pids
float tor_load_s_pid_sp = 1; // the linear force induced by the stimulator, unit: g
float vel_enc_pid_sp = 100; // given as linear velocity, unit: mm/s

// define objects for the 2 motors.
motor tor_motor;
motor vel_motor;

// define the 2 sensors, the load cell and also motor encoders.
load_cell load_s;
encoder vel_enc;

// 2 functions used as isr handlers for hardware interrupts from encoders used for velocity control.
void vel_update_a()
{
  // One of the 2 methods used to internally increment rotation state.
  vel_enc.measure_a(); 
}

void vel_update_b()
{
  vel_enc.measure_b();
}

/* The following two functions were for handling encoders for torque control. But no longer used.
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
/*
// current_sensor tor_curr;
// current_sensor vel_curr;

// pid_controller tor_curr_pid;
// pid_controller vel_curr_pid;
*/

// Define the pid objects for torque and velocity controls.
pid_controller tor_load_s_pid;
pid_controller vel_enc_pid;

// Two flags, used with timer interrupts. 
// This ensures that the control and measurements are updated at desired intervals.
volatile bool updated_flag = false;
volatile bool measured_flag = false;

// 2 functions used as isr handlers for timer interrupts, only updates the respective flags.
void update_updated(){
  updated_flag = false;
}

void update_measured(){
  measured_flag = false;
}

// A function that includes all necessary functions for making raw measurements from sensors. 
// Because encoder uses hardware interrupts, so it is not included here.
// The current sensors are not implemented right now. (commented out)
void raw_measures()
{
  load_s.raw_measure(); // load cell measurements
  // tor_curr.raw_measure();
  // vel_curr.raw_measure();
}

// A function that includes all necessary functions for converting the raw measurement to usable data.
// For load cell, the raw analog value is converted to measurement in gram.
// For velocity encoder, the internally incremented count is converted to rotational & linear velocity in mm/s.
void update_values()
{
  load_s.update_value();
  vel_enc.update_value();
  // tor_enc.update_value();
  // tor_curr.update_value();
  // vel_curr.update_value();
}

// A function that includes all necessary functions for making updates to pid controls.
void update_pid()
{
  tor_load_s_pid.update(); // updates the pid by 1 step
  vel_enc_pid.update(); // updates the pid by 1 step 
  // tor_curr_pid.update();
  // vel_curr_pid.update();
}

// A function that actually actuates the motors.
void update_motors()
{
  // tor_motor.operate(tor_curr_pid.get_output());
  // vel_motor.operate(vel_enc_pid.get_output());
  // tor_motor.operate(tor_load_s_pid.get_output());
  vel_motor.operate(); // operates the velocity motor. The pwm value is updated via internal pointer.
  tor_motor.operate(); // operates the torque motor. 
  // tor_motor.operate(255);
//   vel_motor.operate(255);
}

void setup() {
  Serial.begin(9600);
  // load_s.setup(load_DAT, load_CLK);
  // put your setup code here, to run once:
  
  // setup motors, with 3 output pins, and a pointer to the motor pwm output from the pid controller.
  vel_motor.setup(vel_input_1, vel_input_2, vel_input_pwm, vel_enc_pid.get_p_output());
  tor_motor.setup(tor_input_1, tor_input_2, tor_input_pwm, tor_load_s_pid.get_p_output());

  // setup load cell, with data and clock pins, and coefficients for converting analog value to weight in grams.
  load_s.setup(load_DAT, load_CLK, load_s_coef);
  // setup encoder, with 2 hardware interrupt pins, and its pulse per rotation -> available from datasheet.
  vel_enc.setup(vel_encoder_1, vel_encoder_2, encoder_ppr);
  /* current updates.
  tor_enc.setup(tor_encoder_1, tor_encoder_2, encoder_ppr);
  tor_curr.setup(tor_curr_input);
  vel_curr.setup(vel_curr_input);
  */

  // setup pid controlls, w/ pointers to measurements, setpoints, and also the array of coefficients.
  tor_load_s_pid.setup(load_s.get_p_value(), &tor_load_s_pid_sp, tor_load_s_pid_coef);
  vel_enc_pid.setup(vel_enc.get_p_value(), &vel_enc_pid_sp, vel_enc_pid_coef);
  // tor_curr_pid.setup(tor_curr.get_p_value(), tor_load_s_pid.get_p_output(), tor_curr_pid_coef);
  // vel_curr_pid.setup(vel_curr.get_p_value(), vel_enc_pid.get_p_output(), vel_curr_pid_coef);

  // Limits the output range of velocity encoder, which prevents the motor from rotating backwards.
  vel_enc_pid.set_output_range(0.0, 255.0);

  // activate the pid to start processing/control.
  tor_load_s_pid.activate();
  vel_enc_pid.activate();
  // tor_curr_pid.activate();
  // vel_curr_pid.activate();
  
  // attachInterrupt(digitalPinToInterrupt(tor_encoder_1), tor_update_a, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(tor_encoder_2), tor_update_b, CHANGE);

  // Attach the hardware interrupts for the encoder.
  attachInterrupt(digitalPinToInterrupt(vel_encoder_1), vel_update_a, CHANGE);
  attachInterrupt(digitalPinToInterrupt(vel_encoder_2), vel_update_b, CHANGE);
  // Setup the timer interrupts for update and measurements
  Timer3.initialize(update_per);
  Timer3.attachInterrupt(update_updated);
  Timer1.initialize(measure_per);
  Timer1.attachInterrupt(update_measured);
}


void loop() {
  // put your main code here, to run repeatedly:
  if (!measured_flag){ 
    // If measured_flag is set to false by the interrupt function, it is time to measure.
    raw_measures(); // call to measure.
    measured_flag = true; // set the flag to true.
  }
  if(!updated_flag){
    // So if measured_flag is set to false by the interrupt, the controlls will be updated;
    update_values(); 
    update_pid();
    update_motors();
    updated_flag = true; // set the flag to false.

    /********** Output to serial for debugg/control purposes **********/
    Serial.print("Velocity Encoder: ");
    Serial.println(vel_enc.get_value());
    
    Serial.print("velocity pid output: ");
    Serial.println(vel_enc_pid.get_output());
    
    // Serial.print("velocity current sensor: ");
    // Serial.println(vel_curr.get_value());
    
    Serial.print("Load Cell: ");
    Serial.println(load_s.get_value());

    Serial.print("torque pid output: ");
    Serial.println(tor_load_s_pid.get_output());
    
    /*
    Serial.println(vel_enc.get_value());
    */
  }

}
