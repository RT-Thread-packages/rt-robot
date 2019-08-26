/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __SERVO_H__
#define __SERVO_H__

#include <rtthread.h>
#include <rtdevice.h>

// 20ms; 1.5ms; +-1ms
#define SERVO_PERIOD                    20000000
#define SERVO_DEFAULT_PULSE_MAX         2500000
#define SERVO_DEFAULT_PULSE_MIN         500000

typedef struct servo *servo_t;

struct servo
{
    struct rt_device_pwm    *pwmdev;
    int                     channel;
    float                   angle_maximum;
    rt_uint32_t             pluse_maximum;
    rt_uint32_t             pluse_minimum;
};

servo_t  servo_create(const char * pwm, int channel, float angle, rt_uint32_t pluse_min, rt_uint32_t pluse_max);
rt_err_t servo_destroy(servo_t servo);
rt_err_t servo_enable(servo_t servo);
rt_err_t servo_disable(servo_t servo);
rt_err_t servo_reset(servo_t servo);
rt_err_t servo_set_angle(servo_t servo, float angle);

#endif //  __SERVO_H__
