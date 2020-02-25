# ifndef __CONFIG_H__
# define __CONFIG_H__


namespace stimulator{
  const float drum_diameter = 80; // drum diameter in mm
  const int encoder_ppr = 600; // pulse per rotation of the encoder, from the spec sheet.
  const float update_per = 200000; // the period used by the timerOne library, in micro seconds.
  const float measure_per = 10000;
  const int load_s_coef = 1450;


  // These pins need to be updated based on the ditial layouts
  const int tor_input_1 = 12;
  const int tor_input_2 = 7;
  const int tor_input_pwm = 13;

  const int vel_input_1 = 5;
  const int vel_input_2 = 6;
  const int vel_input_pwm = 4;

  const int tor_encoder_1 = 3;
  const int tor_encoder_2 = 19;

  const int vel_encoder_1 = 2;
  const int vel_encoder_2 = 18;

  const int tor_curr_input = A2;
  const int vel_curr_input = A1;

  const int load_CLK = A4;
  const int load_DAT = A3;

  // The coefficients for pid, in the order of kP, n_kI, n_kD, n_KF
  const float tor_load_s_pid_coef[4] = {1,1,1,1};
  const float tor_curr_pid_coef[4] = {1,1,1,1};
  const float vel_enc_pid_coef[4] = {1,1,1,1};
  const float vel_curr_pid_coef[4] = {1,1,1,1};
  
  // The setpoints for load cell and encoder pids
  const float tor_load_s_pid_sp = 1; // Should be in g
  const float vel_enc_pid_sp = 10; // So, given as the linear velocity, in mm/s
}


# endif
