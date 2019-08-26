/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "single_pwm_motor.h"

#define DBG_SECTION_NAME  "single_pwm_motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

struct single_pwm_motor
{
    struct motor mot;
    struct rt_device_pwm *pwm_dev;
    int channel;
    rt_base_t pin1;
    rt_base_t pin2;
};

static rt_err_t single_pwm_motor_enable(void *mot)
{
    single_pwm_motor_t mot_sub = (single_pwm_motor_t)mot;
    
    rt_pwm_enable(mot_sub->pwm_dev, mot_sub->channel);

    return RT_EOK;
}

static rt_err_t single_pwm_motor_disable(void *mot)
{
    single_pwm_motor_t mot_sub = (single_pwm_motor_t)mot;

    rt_pwm_disable(mot_sub->pwm_dev, mot_sub->channel);

    return RT_EOK;
}

static rt_err_t single_pwm_motor_reset(void *mot)
{
    return RT_EOK;
}

static rt_err_t single_pwm_motor_set_speed(void *mot, rt_int16_t thousands)
{
    single_pwm_motor_t mot_sub = (single_pwm_motor_t)mot;

    if (thousands == 0)
    {
        rt_pwm_set(mot_sub->pwm_dev, mot_sub->channel, MOTOR_PWM_PERIOD, 0);
        rt_pin_write(mot_sub->pin1, PIN_LOW);
        rt_pin_write(mot_sub->pin2, PIN_LOW);
    }
    else
    {
        if (thousands < 0)
        {
            thousands = -thousands;
            rt_pin_write(mot_sub->pin1, PIN_HIGH);
            rt_pin_write(mot_sub->pin2, PIN_LOW);
        }
        else
        {
            rt_pin_write(mot_sub->pin1, PIN_LOW);
            rt_pin_write(mot_sub->pin2, PIN_HIGH);
        }
        
        rt_uint32_t pluse = MOTOR_PWM_PERIOD * thousands / 1000;
        if (pluse > MOTOR_PWM_PERIOD)
        {
            pluse = MOTOR_PWM_PERIOD;
        }
        rt_pwm_set(mot_sub->pwm_dev, mot_sub->channel, MOTOR_PWM_PERIOD, pluse);
    }

    return RT_EOK;
}


single_pwm_motor_t single_pwm_motor_create(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2)
{
    single_pwm_motor_t new_motor = (single_pwm_motor_t)motor_create(sizeof(struct single_pwm_motor));
    if (new_motor == RT_NULL)
    {
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
    new_motor->mot.enable = single_pwm_motor_enable;
    new_motor->mot.disable = single_pwm_motor_disable;
    new_motor->mot.reset = single_pwm_motor_reset;
    new_motor->mot.set_speed = single_pwm_motor_set_speed;

    rt_pin_mode(new_motor->pin1, PIN_MODE_OUTPUT);
    rt_pin_mode(new_motor->pin2, PIN_MODE_OUTPUT);

    return new_motor;
}
