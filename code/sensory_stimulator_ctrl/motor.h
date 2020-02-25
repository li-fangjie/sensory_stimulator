#ifndef __MOTOR_H__
#define __MOTOR_H__


class motor 
{
  private:
    int output_pin_1 = 0;
    int output_pin_2 = 0;
    int output_pin_pwm = 0;
    float * p_input;
    // float input = 0;
    // bool input_set = false;

  public:
    motor(){};
    void setup(int pin_1, int pin_2, int pin_pwm);

    void setup(int pin_1, int pin_2, int pin_pwm, float * n_p_input);
    
    void operate(float val);

    void operate();
    
    float* get_p_input();
};


#endif
