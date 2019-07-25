#ifndef __SINGLE_PWM_MOTOR_H__
#define __SINGLE_PWM_MOTOR_H__

#include "motor.h"

typedef struct single_pwm_motor *single_pwm_motor_t;

struct single_pwm_motor
{
    struct motor mot;
    struct rt_device_pwm *pwm_dev;
    int channel;
    rt_base_t pin1;
    rt_base_t pin2;
};

single_pwm_motor_t  single_pwm_motor_create(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2);

#endif // __SINGLE_PWM_MOTOR_H__
