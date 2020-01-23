#ifndef __MOTOR_H__
#define __MOTOR_H__


class motor 
{
  private:
    int output_pin;

  public:
    motor();
    void set_up(int pin)
    {
      output_pin = pin;
    }

    void operate(int val)
    {
      analogWrite(output_pin, val);
    }
};


#endif
