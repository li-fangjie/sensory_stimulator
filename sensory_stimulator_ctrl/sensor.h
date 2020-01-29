#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "HX711.h" //
// Please refer to: https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide

#define calibration_factor -7050.0 // Calibration factor, which we need to acquire with from calibration tests.

/*A sensor or encoder object should input and raw value, 
  and process them such that they can be used directly in the control system*/

class sensor_interface
{
  protected:
  float cur_val;
  float cur_out;
  
  public:
  /*
  void setup();
  
  void measure();
  
  virtual void update_value();
  
  virtual float get_value();
  
  virtual float* get_p_value();
  */
};

class sensor : public sensor_interface
{ 
  protected:
    int input_pin;
    int cur_raw_count;
    int raw_val_s[40];

  public:
    sensor();
    void setup(int pin);

    void raw_measure();

    void measure();

    void update_value();

    float get_value();
    // Your typical and lonely getter method

    float* get_p_value();
};

class load_cell : public sensor
{
  int CLK;
  HX711 scale;
  
  public:
    load_cell();
    void setup(int n_DOUT, int n_CLK);

    void measure();

    void update_value();
};

class current_sensor : public sensor
{
  private:
  int offset_mv = 2500;
  int sensitivity_mv = 185;
  
  public:
    current_sensor();

    void update_value();
};

class encoder : private sensor_interface
{
  private:
    int input_pin_a;
    int input_pin_b;
    int ppr; // pulse per rotation

    volatile int cur_pulse_count;
    volatile int cur_rotat_count;
    float time_period; // The time elapsed from last calculation, in microseconds.
    float c;
    float cur_val;
    float cur_rps;

  public:
    encoder();
    void setup(int pin_a, int pin_b, int pulse_per_rotation);

    void setup(int pin_a, int pulse_per_rotation);

    void measure_a(); 
    // update_pulse_count for A phase
    // This method handles interrupts from encoders
    // sorts out pulse count.


    void measure_b();
    // update_pulse_count for B phase
    // This method handles interrupts from encoders
    // sorts out pulse count.

    void update_velocity();
    // This method handles interrupts from timer
    // calculates linear velocity.
 
    void update_value();

    float get_value();
    // Your typical and lonely getter method

    float * get_p_value();

    float get_rpm();
};


#endif
