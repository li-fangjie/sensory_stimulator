# ifndef __CONTROL_2_H__
# define __CONTROL_2_H__

template <class T>
void copy(T* dst, T* src, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

class control_2
{
  private:
    int output_range[2] = {0, 255}; // The range of output to be used.
    int * output;
    int * inputs; // First input should be sensor reading, others can be PID readings from upstream
    float * coefficients; // coefficients. {k_p, k_i, k_d, k_input_pid_1, k_input_pid_2, ...}
    int input_count; // number of inputs
    
    float i_error; // sum of errors for the integral term
    float prev_error;
    // The percentage range below or above the set point, 
    // where the integral coefficient is included. To prevent integral windup.
    
    float k_i_range; 
    float sp; // set_point
    bool state; // whether the PID is turned on or not.

  public:
    pid();

    void set_up(int * new_inputs, int new_input_count, int * new_output, float * k, float new_k_i_range)
    {
      copy(inputs, new_inputs, input_count);
      copy(coefficients, k, input_count+2);
      input_count = new_input_count;
      output = new_output;
      if (new_k_i_range> 0 && new_k_i_range <= 1) k_i_range = new_k_i_range;
    }

    void update()
    {
      if(!state) return;
      int cur_input = inputs[0];  
      float cur_output;
      
      float p_error = (float)cur_input - sp;
      i_error += p_error;
      float d_error = p_error - prev_error;
      
      float error_terms[3] = {p_error, i_error, d_error};
      if(abs(sp - cur_input)/sp < k_i_range){
          i_error = 0;
          error_terms[2] = 0;
        }
      for(int i=0; i<=3; i++){
        cur_output += error_terms[i] * coefficients[i];
        }

      for(int i=0; i<input_count-1; i++){
        cur_output += inputs[i+1] * coefficients[i+3];
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

    
    void tune(float * k)
    {
      copy(coefficients, k, input_count+2);
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
