#ifndef __CONTROL_4_H__
#define __CONTROL_4_H__

class pid_controller
{
    private:
        float * input;
        float * output;
        float kP, kI, kD, kF;
        float * sp;
        float resolution = 0;
        float out_range[2] = {-255, 255};
        
        float i_error;
        float prev_error;
        
        bool state;

    public:
        void setup(float* n_input, float* n_sp, float n_kP, float n_kI, float n_kD, float n_KF);

        void setup(float* n_input, float* n_sp, float k[4]);

        void setup(float* n_input, float* n_output, float* n_sp, float k[4]);

        void set_resolution(float reso);

        void set_output_range(float lower, float upper);

        void activate();

        void pause();

        void update();

        void post_process();

        void set_coef(float k_p, float k_i, float k_d, float k_f);

        float* get_p_output();

        float get_output();

        float get_kP();

        float get_kI();
        
        float get_kD();

        float get_kF();

        void terminate();
};

#endif
