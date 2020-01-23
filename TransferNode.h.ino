# ifndef __CONTROL_H__
# define __CONTROL_H__


class TransferNode
{
  private:
    float value;

  public:
    TransferNode();

    void init(float initValue)
    {
      value = initValue;
    }

    void init() {
      value = 0;
    }

    float pidGet() {
      return value;
    }

    float setOutput(float newValue) {
      value = newValue;
    }
};


# endif
