# ifndef __CONFIG_H__
# define __CONFIG_H__


namespace stimulator{
  float drum_diameter = 80; // drum diameter in mm
  int encoder_ppr = 600; // pulse per rotation of the encoder, from the spec sheet.
  int timer_period = 10000; // the period used by the timerOne library, in micro seconds.
}


# endif
