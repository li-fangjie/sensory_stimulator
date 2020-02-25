# ifndef __TRANSFER_NODE_H__
# define __TRANSFER_NODE_H__


class transfer_node
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

    void set(float val) {
      value = val;
    }

    float setOutput(float newValue) {
      value = newValue;
    }
};


# endif
