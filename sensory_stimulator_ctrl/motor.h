#ifndef __MOTOR_H__
#define __MOTOR_H__


class motor 
{
  private:
    int output_pin_1;
    int output_pin_2;
    int output_pin_pwm;
    float * input;
    bool input_set = false;

  public:
    motor(){};
    void setup(int pin_1, int pin_2, int pin_pwm);

    void setup(int pin_1, int pin_2, int pin_pwm, float * n_input);

    void operate(int val);

    void operate();
};


#endif
