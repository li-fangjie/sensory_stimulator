#ifndef __MOTOR_H__
#define __MOTOR_H__


class motor 
{
  private:
    int output_pin_1;
    int output_pin_2;
    int output_pin_pwm;

  public:
    motor(){};
    void setup(int pin_1, int pin_2, int pin_pwm);

    void operate(int val);
};


#endif
