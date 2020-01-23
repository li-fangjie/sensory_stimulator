#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "config.h"


/*A sensor or encoder object should input and raw value, 
  and process them such that they can be used directly in the control system*/
class sensor 
{ 
  private:
    int input_pin;
    int current_value;

  public:
    sensor();
    void set_up(int pin)
    {
      input_pin = pin;
    }

    void read()
    {
      current_value = analogRead(input_pin);
    }

    int get_value()
    // Your typical and lonely getter method
    {
      return current_value;
    }
};

class load_cell : sensor
{

};

class current_sensor : sensor
{
  
};

class decoder {
  private:
    int input_pin_a;
    int input_pin_b;
    int ppr; // pulse per rotation

    volatile int cur_pulse_count;
    volatile int cur_rotat_count;
    int cur_v;

  public:
    Decoder();
    void set_up(int pin_a, int pin_b, int pulse_per_rotation)
    {
      input_pin_a = pin_a;
      input_pin_b = pin_b;
      ppr = pulse_per_rotation * 4; // With two interrupts, each capable of counting rising+falling edges, the precision can be quadrupled.
    }

    void set_up(int pin_a, int pulse_per_rotation)
    {
      input_pin_a = pin_a;
      ppr = pulse_per_rotation * 2; // same as above, but with 1 inerrupt, precision is doubled;
    }

    void update_pulse_count() 
    // This method handles interrupts from encoders
    // sorts out pulse count.
    {
      ++ cur_pulse_count;
      if (cur_pulse_count >= ppr){
        cur_pulse_count = 0;
        ++ cur_rotat_count;
      }
    }

    void update_velocity()
    // This method handles interrupts from timer
    // calculates linear velocity.
    {
      float time_period = static_cast<float>(stimulator::timer_period * 1000000); // The time elapsed from last calculation, in seconds.
      // The distance travelled from last calculation, in mm.
      float distance_travelled = ((float)cur_rotat_count + (float)cur_pulse_count/(float)ppr) * (float)stimulator::drum_diameter * 3.1415926;
      cur_v = distance_travelled / time_period;
      cur_rotat_count = 0; // resets rotation count for next speed measurements.
    }

    float get_value()
    // Your typical and lonely getter method
    {
      return cur_v;
    }
};


#endif
