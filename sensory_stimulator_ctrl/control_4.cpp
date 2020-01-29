#include "control_4.h"
#include "Arduino.h"


void pid_controller::setup(float* n_input, float* n_sp, float n_kP, float n_kI, float n_kD, float n_KF)
{
    input = n_input;
    sp = n_sp;
    kP = n_kP;
    kI = n_kI;
    kD = n_kD;
    kF = n_KF;
    output = new float;
}

void pid_controller::setup(float* n_input, float* n_sp, float k[4])
{
  input = n_input;
  sp = n_sp;
  kP = k[0];
  kI = k[1];
  kD = k[2];
  kF = k[3];
  output = new float;
}

void pid_controller::setup(float* n_input, float* n_output, float* n_sp, float k[4])
{
  input = n_input;
  sp = n_sp;
  kP = k[0];
  kI = k[1];
  kD = k[2];
  kF = k[3];
  output = n_output;
}

void pid_controller::set_resolution(float reso)
{
    resolution = reso;
}

void pid_controller::set_output_range(float lower, float upper)
{
    out_range[0] = lower;
    out_range[1] = upper;
}

void pid_controller::post_process()
{
    if(*output < out_range[0]) *output = out_range[0];
    if(*output < out_range[1]) *output = out_range[1];   
}

void pid_controller::activate()
{
    state = true;
    i_error = 0;
    prev_error = 0;
}

void pid_controller::pause()
{
    state = false;
}

void pid_controller::update()
{
    if(!state) return;
    float error = *sp - *input;
    i_error += error;

    if (error < resolution){
        error = 0;
        i_error = 0;
    }

    float p_term = kP * error;
    float i_term = kI * kP * i_error;
    float d_term = kD * kP * (error - prev_error);
    float f_term = kF * (*sp);
    *output = p_term + i_term + d_term + f_term;
    
    post_process();
    prev_error = error;
}

void pid_controller::set_coef(float k_p, float k_i, float k_d, float k_f)
{
    kP = k_p;
    kI = k_i;
    kD = k_d;
    kF = k_f;
}

float* pid_controller::get_p_output()
{
    return output;
}

float pid_controller::get_output()
{
    return *output;
}

float pid_controller::get_kP()
{
    return kP;
}

float pid_controller::get_kI()
{
    return kI;
}

float pid_controller::get_kD()
{
    return kD;
}

float pid_controller::get_kF()
{
    return kF;
}

void pid_controller::terminate()
{
  delete output;
}
