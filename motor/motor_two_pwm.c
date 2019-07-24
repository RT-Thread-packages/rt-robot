#include "motor_two_pwm.h"

#define DBG_SECTION_NAME "motor_two_pwm"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

motor_t motor_two_pwm_creat(char *pwm1, int pwm1_channel, char *pwm2, int pwm2_channel)
{
    motor_two_pwm_t new_motor = (motor_two_pwm_t)rt_malloc(sizeof(struct motor_two_pwm));
    if (new_motor == RT_NULL)
    {
        LOG_E("Falied to allocate memory for motor");
        return RT_NULL;
    }

    new_motor->pwm1_dev = (struct rt_device_pwm*)rt_device_find(pwm1);
    if (new_motor->pwm1_dev == RT_NULL)
    {
        rt_free(new_motor);
        LOG_E("Falied to find device on %s", pwm1);
        return RT_NULL;
    }
    new_motor->pwm2_dev = (struct rt_device_pwm*)rt_device_find(pwm2);
    if (new_motor->pwm2_dev == RT_NULL)
    {
        rt_free(new_motor);
        LOG_E("Falied to find device on %s", pwm2);
        return RT_NULL;
    }
    new_motor->pwm1_channel = pwm1_channel;
    new_motor->pwm2_channel = pwm2_channel;
    new_motor->mot.enable = motor_two_pwm_enable;
    new_motor->mot.disable = motor_two_pwm_disable;
    new_motor->mot.set_speed = motor_two_pwm_set_speed;

    return &new_motor->mot;
}

rt_err_t motor_two_pwm_enable(motor_t mot)
{
    motor_two_pwm_t mot_one_pwm = (motor_two_pwm_t)mot;

    rt_pwm_enable(mot_one_pwm->pwm1_dev, mot_one_pwm->pwm1_channel);
    rt_pwm_enable(mot_one_pwm->pwm2_dev, mot_one_pwm->pwm2_channel);

    return RT_EOK;
}

rt_err_t motor_two_pwm_disable(motor_t mot)
{
    motor_two_pwm_t mot_one_pwm = (motor_two_pwm_t)mot;
    
    rt_pwm_disable(mot_one_pwm->pwm1_dev, mot_one_pwm->pwm1_channel);
    rt_pwm_disable(mot_one_pwm->pwm2_dev, mot_one_pwm->pwm2_channel);

    return RT_EOK;
}

rt_err_t motor_two_pwm_set_speed(motor_t mot, rt_int16_t thousands)
{
    motor_two_pwm_t mot_one_pwm = (motor_two_pwm_t)mot;

    if (thousands == 0)
    {
        rt_pwm_set(mot_one_pwm->pwm1_dev, mot_one_pwm->pwm1_channel, MOTOR_PWM_PERIOD, 0);
        rt_pwm_set(mot_one_pwm->pwm2_dev, mot_one_pwm->pwm2_channel, MOTOR_PWM_PERIOD, 0);
    }
    else if (thousands > 0)
    {
        rt_uint32_t pluse = MOTOR_PWM_PERIOD * thousands / 1000;
        if (pluse > MOTOR_PWM_PERIOD)
        {
            pluse = MOTOR_PWM_PERIOD;
        }
        rt_pwm_set(mot_one_pwm->pwm1_dev, mot_one_pwm->pwm1_channel, MOTOR_PWM_PERIOD, pluse);
        rt_pwm_set(mot_one_pwm->pwm2_dev, mot_one_pwm->pwm2_channel, MOTOR_PWM_PERIOD, 0);
    }
    else
    {
        rt_uint32_t pluse = MOTOR_PWM_PERIOD * -thousands / 1000;
        if (pluse > MOTOR_PWM_PERIOD)
        {
            pluse = MOTOR_PWM_PERIOD;
        }
        rt_pwm_set(mot_one_pwm->pwm1_dev, mot_one_pwm->pwm1_channel, MOTOR_PWM_PERIOD, 0);
        rt_pwm_set(mot_one_pwm->pwm2_dev, mot_one_pwm->pwm2_channel, MOTOR_PWM_PERIOD, pluse);
    }

    return RT_EOK;
}
