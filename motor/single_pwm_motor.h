#ifndef __SINGLE_PWM_MOTOR_H__
#define __SINGLE_PWM_MOTOR_H__

#include "motor.h"

struct single_pwm_motor;
typedef struct single_pwm_motor *single_pwm_motor_t;

single_pwm_motor_t  single_pwm_motor_create(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2);

#endif // __SINGLE_PWM_MOTOR_H__
