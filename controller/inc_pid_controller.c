/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "inc_pid_controller.h"

#define DBG_SECTION_NAME  "inc_pid_controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

static rt_err_t inc_pid_controller_reset(void *pid)
{
    inc_pid_controller_t inc_pid = (inc_pid_controller_t)pid;
    
    inc_pid->error = 0.0f;
    inc_pid->error_l = 0.0f;
    inc_pid->error_ll = 0.0f;
    inc_pid->last_out = 0.0f;

    return RT_EOK;
}

static rt_err_t inc_pid_controller_destroy(void *pid)
{
    rt_free(pid);
    return RT_EOK;
}

static rt_err_t inc_pid_controller_set_param(void *pid, controller_param_t param)
{
    inc_pid_controller_t inc_pid = (inc_pid_controller_t)pid;

    inc_pid->kp = param->data.pid.kp;
    inc_pid->ki = param->data.pid.ki;
    inc_pid->kd = param->data.pid.kd;

    return RT_EOK;
}

static rt_err_t inc_pid_controller_get_param(void *pid, controller_param_t param)
{
    inc_pid_controller_t inc_pid = (inc_pid_controller_t)pid;

    param->data.pid.kp = inc_pid->kp;
    param->data.pid.ki = inc_pid->ki;
    param->data.pid.kd = inc_pid->kd;

    return RT_EOK;
}

static rt_err_t inc_pid_controller_update(void *pid, float current_point)
{
    inc_pid_controller_t inc_pid = (inc_pid_controller_t)pid;
    // TODO
    if((rt_tick_get() - inc_pid->last_time) < rt_tick_from_millisecond(inc_pid->controller.sample_time))
    {
        LOG_D("PID waiting ... ");
        return RT_EBUSY;
    }
    inc_pid->last_time = rt_tick_get();

    inc_pid->error = inc_pid->controller.target - current_point;

    inc_pid->p_error = inc_pid->kp * (inc_pid->error - inc_pid->error_l);
    inc_pid->i_error = inc_pid->ki * inc_pid->error;
    inc_pid->d_error = inc_pid->kd * (inc_pid->error - 2 * inc_pid->error_l + inc_pid->error_ll);

    inc_pid->last_out += inc_pid->p_error + inc_pid->i_error + inc_pid->d_error;

    if (inc_pid->last_out > inc_pid->maximum)
    {
        inc_pid->last_out = inc_pid->maximum;
    }
    if (inc_pid->last_out < inc_pid->minimum)
    {
        inc_pid->last_out = inc_pid->minimum;
    }

    inc_pid->error_ll = inc_pid->error_l;
    inc_pid->error_l = inc_pid->error;

    inc_pid->controller.output = inc_pid->last_out;

    return RT_EOK;
}

inc_pid_controller_t inc_pid_controller_create(float kp, float ki, float kd, rt_uint16_t sample_time)
{
    inc_pid_controller_t new_pid = (inc_pid_controller_t)controller_create(sizeof(struct inc_pid_controller), sample_time);
    if(new_pid == RT_NULL)
    {
        return RT_NULL;
    }

    new_pid->kp = kp;
    new_pid->ki = ki;
    new_pid->kd = kd;

    new_pid->maximum = +1000;
    new_pid->minimum = -1000;

    new_pid->p_error = 0.0f;
    new_pid->i_error = 0.0f;
    new_pid->d_error = 0.0f;

    new_pid->error = 0.0f;
    new_pid->error_l = 0.0f;
    new_pid->error_ll = 0.0f;

    new_pid->last_out = 0.0f;

    new_pid->controller.reset = inc_pid_controller_reset;
    new_pid->controller.destroy = inc_pid_controller_destroy;
    new_pid->controller.update = inc_pid_controller_update;
    new_pid->controller.set_param = inc_pid_controller_set_param;
    new_pid->controller.get_param = inc_pid_controller_get_param;
    
    return new_pid;
}

rt_err_t inc_pid_controller_set_kp(inc_pid_controller_t pid, float kp)
{
    RT_ASSERT(pid != RT_NULL);

    pid->kp = kp;
    return RT_EOK;
}

rt_err_t inc_pid_controller_set_ki(inc_pid_controller_t pid, float ki)
{
    RT_ASSERT(pid != RT_NULL);
    
    pid->ki = ki;
    return RT_EOK;
}

rt_err_t inc_pid_controller_set_kd(inc_pid_controller_t pid, float kd)
{
    RT_ASSERT(pid != RT_NULL);

    pid->kd = kd;
    return RT_EOK;
}
