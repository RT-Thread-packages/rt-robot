#ifndef __DUAL_PWM_MOTOR_H__
#define __DUAL_PWM_MOTOR_H__

#include "motor.h"

struct dual_pwm_motor;
typedef struct dual_pwm_motor *dual_pwm_motor_t;

dual_pwm_motor_t dual_pwm_motor_create(char *pwm1, int pwm1_channel, char *pwm2, int pwm2_channel);

#endif // __DUAL_PWM_MOTOR_H__
