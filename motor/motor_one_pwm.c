#include "motor_one_pwm.h"

#define DBG_SECTION_NAME  "motor_one_pwm"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

motor_t motor_one_pwm_creat(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2)
{
    motor_one_pwm_t new_motor = (motor_one_pwm_t)rt_malloc(sizeof(struct motor_one_pwm));
    if(new_motor == RT_NULL)
    {
        LOG_E("Falied to allocate memory for motor");
        return RT_NULL;
    }

    new_motor->pwm_dev = (struct rt_device_pwm*)rt_device_find(pwm);
    if (new_motor->pwm_dev == RT_NULL)
    {
        rt_free(new_motor);
        LOG_E("Falied to find device on %s", pwm);
        return RT_NULL;
    }
    new_motor->channel = channel;
    new_motor->pin1 = pin1;
    new_motor->pin2 = pin2;
    new_motor->mot.enable = motor_one_pwm_enable;
    new_motor->mot.disable = motor_one_pwm_disable;
    new_motor->mot.set_speed = motor_one_pwm_set_speed;

    rt_pin_mode(new_motor->pin1, PIN_MODE_OUTPUT);
    rt_pin_mode(new_motor->pin2, PIN_MODE_OUTPUT);

    return &new_motor->mot;
}

rt_err_t motor_one_pwm_enable(motor_t mot)
{
    motor_one_pwm_t mot_one_pwm = (motor_one_pwm_t)mot;
    
    rt_pwm_enable(mot_one_pwm->pwm_dev, mot_one_pwm->channel);

    return RT_EOK;
}

rt_err_t motor_one_pwm_disable(motor_t mot)
{
    motor_one_pwm_t mot_one_pwm = (motor_one_pwm_t)mot;

    rt_pwm_disable(mot_one_pwm->pwm_dev, mot_one_pwm->channel);

    return RT_EOK;
}

rt_err_t motor_one_pwm_set_speed(motor_t mot, rt_int16_t thousands)
{
    motor_one_pwm_t mot_one_pwm = (motor_one_pwm_t)mot;

    if (thousands == 0)
    {
        rt_pwm_set(mot_one_pwm->pwm_dev, mot_one_pwm->channel, MOTOR_PWM_PERIOD, 0);
        rt_pin_write(mot_one_pwm->pin1, PIN_LOW);
        rt_pin_write(mot_one_pwm->pin2, PIN_LOW);
    }
    else
    {
        if (thousands < 0)
        {
            thousands = -thousands;
            rt_pin_write(mot_one_pwm->pin1, PIN_HIGH);
            rt_pin_write(mot_one_pwm->pin2, PIN_LOW);
        }
        else
        {
            rt_pin_write(mot_one_pwm->pin1, PIN_LOW);
            rt_pin_write(mot_one_pwm->pin2, PIN_HIGH);
        }
        
        rt_uint32_t pluse = MOTOR_PWM_PERIOD * thousands / 1000;
        if (pluse > MOTOR_PWM_PERIOD)
        {
            pluse = MOTOR_PWM_PERIOD;
        }
        rt_pwm_set(mot_one_pwm->pwm_dev, mot_one_pwm->channel, MOTOR_PWM_PERIOD, pluse);
    }

    return RT_EOK;
}
