/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __INC_PID_CONTROLLER_H__
#define __INC_PID_CONTROLLER_H__

#include <rtthread.h>
#include "controller.h"

typedef struct inc_pid_controller *inc_pid_controller_t;

struct inc_pid_controller
{
    struct controller   controller;
    
    float               kp;
    float               ki;
    float               kd;

    float               minimum;
    float               maximum;

    float               p_error;
    float               i_error;
    float               d_error;

    float               error;
    float               error_l;
    float               error_ll;

    float               last_out;
    rt_tick_t           last_time;
};

inc_pid_controller_t    inc_pid_controller_create(float kp, float ki, float kd, rt_uint16_t sample_time);
rt_err_t                inc_pid_controller_set_kp(inc_pid_controller_t pid, float kp);
rt_err_t                inc_pid_controller_set_ki(inc_pid_controller_t pid, float ki);
rt_err_t                inc_pid_controller_set_kd(inc_pid_controller_t pid, float kd);

#endif
