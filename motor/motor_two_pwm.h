#ifndef __MOTOR_TWO_PWM_H__
#define __MOTOR_TWO_PWM_H__

#include "motor.h"

typedef struct motor_two_pwm *motor_two_pwm_t;

struct motor_two_pwm
{
    struct motor mot;
    struct rt_device_pwm *pwm1_dev;
    int pwm1_channel;
    struct rt_device_pwm *pwm2_dev;
    int pwm2_channel;
};

motor_t  motor_two_pwm_creat(char *pwm1, int pwm1_channel, char *pwm2, int pwm2_channel);
rt_err_t motor_two_pwm_enable(motor_t mot);
rt_err_t motor_two_pwm_disable(motor_t mot);
rt_err_t motor_two_pwm_set_speed(motor_t mot, rt_int16_t thousands);

#endif // __MOTOR_TWO_PWM_H__
