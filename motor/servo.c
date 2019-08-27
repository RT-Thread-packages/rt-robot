/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "servo.h"

#define DBG_SECTION_NAME    "servo"
#define DBG_LEVEL           DBG_LOG
#include <rtdbg.h>

/**
 * @brief create a servo object
 * @param pwm pwm device name
 * @param channel pwm channel
 * @param angle servo angle range
 * @param pluse_min the minimum of pwm pluse width that servo can receive   (unit:ns
 * @param pluse_max the maximum of pwm pluse width that servo can receive   (unit:ns
 * @return the pointer of structure servo if success
 */
servo_t servo_create(const char * pwm, int channel, float angle, rt_uint32_t pluse_min, rt_uint32_t pluse_max)
{
    // Malloc memory for new servo
    servo_t new_servo = (servo_t) rt_malloc(sizeof(struct servo));
    if(new_servo == RT_NULL)
    {
        LOG_E("Falied to allocate memory for new servo");
        return RT_NULL;
    }

    new_servo->pwmdev = (struct rt_device_pwm *)rt_device_find(pwm);
    if (new_servo->pwmdev == RT_NULL)
    {
        LOG_E("Falied to find device on %s", pwm);
        servo_destroy(new_servo);
        return RT_NULL;
    }
    new_servo->channel = channel;
    new_servo->angle_maximum = angle;
    if (pluse_max == RT_NULL || pluse_min == RT_NULL)
    {
        new_servo->pluse_maximum = SERVO_DEFAULT_PULSE_MAX;
        new_servo->pluse_minimum = SERVO_DEFAULT_PULSE_MIN;
    }
    else
    {
        new_servo->pluse_maximum = pluse_max;
        new_servo->pluse_minimum = pluse_min;
    }

    return new_servo;
}

rt_err_t servo_destroy(servo_t servo)
{
    RT_ASSERT(servo != RT_NULL);

    LOG_D("Free servo");
    rt_free(servo);

    return RT_EOK;
}

rt_err_t servo_enable(servo_t servo)
{
    RT_ASSERT(servo != RT_NULL);

    LOG_D("Enabling servo");

    return rt_pwm_enable(servo->pwmdev, servo->channel);
}

rt_err_t servo_disable(servo_t servo)
{
    RT_ASSERT(servo != RT_NULL);

    LOG_D("Disabling servo");

    return rt_pwm_disable(servo->pwmdev, servo->channel);
}

rt_err_t servo_reset(servo_t servo)
{
    return RT_EOK;
}

rt_err_t servo_set_angle(servo_t servo, float angle)
{
    RT_ASSERT(servo != RT_NULL);
    
    rt_uint32_t set_point = servo->pluse_minimum + (servo->pluse_maximum - servo->pluse_minimum) * angle / servo->angle_maximum;
    
    return rt_pwm_set(servo->pwmdev, servo->channel, SERVO_PERIOD, set_point);
}
