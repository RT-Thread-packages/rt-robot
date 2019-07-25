#ifndef __DUAL_PWM_MOTOR_H__
#define __DUAL_PWM_MOTOR_H__

#include "motor.h"

motor_t  dual_pwm_motor_create(char *pwm1, int pwm1_channel, char *pwm2, int pwm2_channel);

#endif // __DUAL_PWM_MOTOR_H__
