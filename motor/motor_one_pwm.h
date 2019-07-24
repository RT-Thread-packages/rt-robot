#ifndef __MOTOR_ONE_PWM_H__
#define __MOTOR_ONE_PWM_H__

#include "motor.h"

typedef struct motor_one_pwm *motor_one_pwm_t;

struct motor_one_pwm
{
    struct motor mot;
    struct rt_device_pwm *pwm_dev;
    int channel;
    rt_base_t pin1;
    rt_base_t pin2;
};

motor_t  motor_one_pwm_creat(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2);
rt_err_t motor_one_pwm_enable(motor_t mot);
rt_err_t motor_one_pwm_disable(motor_t mot);
rt_err_t motor_one_pwm_set_speed(motor_t mot, rt_int16_t thousands);

#endif // __MOTOR_ONE_PWM_H__
