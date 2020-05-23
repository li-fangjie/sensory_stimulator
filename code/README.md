# Sensory Stimulator Drum Control SYstem
### System Outline
This document outlines the components and operations of the software that controls the stimulator drum. Overall, the control softaware aims to control the rotational velocity and torque of the drum via 2 separate PID control system. Each system consists of sensor, PID loop, and a motor effector. 

### Operation Instruction
##### PID Tuning
The coefficients of PID are stored in `config.h`, as a single array of length 4: `{kP, kI, kD KF}`. `tor_load_s_pid_coef` stores the coefficients for torque PID with load cell as input; `vel_enc_pid_coef` stores the coefficients for velocity PID with the encoder as input.

The PIDs can be tuned by adjusting the coefficients stored in those variables. Currently, this means each adjustment requires recompiling the code.

##### Data Collection
