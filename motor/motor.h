/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <rtthread.h>
#include <rtdevice.h>

// 20KHz
#define MOTOR_PWM_PERIOD 50000

typedef struct motor *motor_t;

struct motor
{
    rt_err_t    (*enable)(void *mot);
    rt_err_t    (*disable)(void *mot);
    rt_err_t    (*reset)(void *mot);
    rt_err_t    (*set_speed)(void *mot, rt_int16_t thousands);
};

motor_t  motor_create(rt_size_t size);
rt_err_t motor_destroy(motor_t mot);
rt_err_t motor_reset(motor_t mot);
rt_err_t motor_enable(motor_t mot);
rt_err_t motor_disable(motor_t mot);
rt_err_t motor_run(motor_t motor, rt_int16_t thousands);
rt_err_t motor_stop(motor_t motor);

#endif
