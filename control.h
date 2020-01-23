# ifndef __CONTROL_H__
# define __CONTROL_H__


class control
{
  private:
    int output_range[2] = {0, 255}; // The range of output to be used.
    int * output;
    int * input;
    int * input_pid; // input from first layer PID controller.
    float coefficients[4]; // coefficients. {k_p, k_i, k_d, k_input_pid}
    
    float i_error; // sum of errors for the integral term
    float prev_error;
    // The percentage range below or above the set point, 
    // where the integral coefficient is included. To prevent integral windup.
    
    float k_i_range; 
    float sp; // set_point
    bool state; // whether the PID is turned on or not.

  public:
    pid();

    void set_up(int * new_input, int * new_input_pid, int * new_output, float k_p, float k_i, float k_d, float k_input_pid, float new_k_i_range)
    {
      input_pid = new_input_pid;
      input = new_input;
      output = new_output;
      coefficients[0] = k_p;
      coefficients[1] = k_i;
      coefficients[2] = k_d;
      coefficients[3] = k_input_pid;
      if (new_k_i_range> 0 && new_k_i_range <= 1) k_i_range = new_k_i_range;
    }

    void set_up(int * new_input, int * new_output, float k_p, float k_i, float k_d, float new_k_i_range)
    {
      input = new_input;
      output = new_output;
      coefficients[0] = k_p;
      coefficients[1] = k_i;
      coefficients[2] = k_d;
      if (new_k_i_range> 0 && new_k_i_range <= 1) k_i_range = new_k_i_range;
    }

    void update()
    {
      if(!state) return;
      int cur_input = *input;  
      float cur_output;
      
      float p_error = (float)cur_input - sp;
      i_error += p_error;
      float d_error = p_error - prev_error;
      float cur_input_pid = (float)*input_pid;
      
      float error_terms[4] = {p_error, i_error, d_error, cur_input_pid};
      
      for(int i=0; i<=4; i++){
        if(i=1 && abs(sp - cur_input)/sp < k_i_range){
          i_error = 0;
          continue;
        }
        cur_output += error_terms[i] * coefficients[i];
        }

      *output = process_output(cur_output);
      prev_error = p_error;
    }

    int process_output(float cur_output)
    // A method to do post processing for the output.
    {
      if(cur_output <= output_range[0]) cur_output = 0;
      else if(cur_output <= output_range[1]) cur_output = 255;
      
      int output = round(cur_output); // this is floor(), way want to change this to 
      return output;
    }

    
    void tune(float k_p, float k_i, float k_d, float k_input_pid)
    {
      coefficients[0] = k_p;
      coefficients[1] = k_i;
      coefficients[2] = k_d;
      coefficients[3] = k_input_pid;
    }

    void tune(float k_p, float k_i, float k_d)
    {
      coefficients[0] = k_p;
      coefficients[1] = k_i;
      coefficients[2] = k_d;
    }

    void set_sp(float new_sp)
    {
      sp = new_sp;
    }

    void set_range(int min_val, int max_val)
    {
      output_range[0] = min_val;
      output_range[1] = max_val;
    }

    void activate()
    {
      state = true;
    }

    void deactivate()
    {
      state = false;
      i_error = 0;
      prev_error = 0;
    }

    void get_coefficient(int index)
    {
      if (index > 4) return 0;
      return coefficients[index];
    }
};


# endif
