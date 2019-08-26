/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "pos_pid_controller.h"

#define DBG_SECTION_NAME  "pos_pid_controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

static rt_err_t pos_pid_controller_reset(void *pid)
{
    pos_pid_controller_t pos_pid = (pos_pid_controller_t)pid;

    pos_pid->error = 0.0f;
    pos_pid->error_l = 0.0f;
    pos_pid->integral = 0.0f;
    pos_pid->last_out = 0.0f;

    return RT_EOK;
}

static rt_err_t pos_pid_controller_destroy(void *pid)
{
    rt_free(pid);
    return RT_EOK;
}

static rt_err_t pos_pid_controller_set_param(void *pid, controller_param_t param)
{
    pos_pid_controller_t pos_pid = (pos_pid_controller_t)pid;

    pos_pid->kp = param->data.pid.kp;
    pos_pid->ki = param->data.pid.ki;
    pos_pid->kd = param->data.pid.kd;

    return RT_EOK;
}

static rt_err_t pos_pid_controller_get_param(void *pid, controller_param_t param)
{
    pos_pid_controller_t pos_pid = (pos_pid_controller_t)pid;

    param->data.pid.kp = pos_pid->kp;
    param->data.pid.ki = pos_pid->ki;
    param->data.pid.kd = pos_pid->kd;

    return RT_EOK;
}

static rt_err_t pos_pid_controller_update(void *pid, float current_point)
{
    pos_pid_controller_t pos_pid = (pos_pid_controller_t)pid;

    if((rt_tick_get() - pos_pid->last_time) < rt_tick_from_millisecond(pos_pid->controller.sample_time))
    {
        LOG_D("PID waiting ... ");
        return RT_EBUSY;
    }
    pos_pid->last_time = rt_tick_get();

    pos_pid->error = pos_pid->controller.target - current_point;

    pos_pid->integral += pos_pid->ki * pos_pid->error;

    //Perform integral value capping to avoid internal PID state to blows up
    //when controllertuators saturate:
    if(pos_pid->integral > pos_pid->anti_windup_value) {
        pos_pid->integral = pos_pid->anti_windup_value;
    } else if (pos_pid->integral < -pos_pid->anti_windup_value) {
        pos_pid->integral = -pos_pid->anti_windup_value;
    }

    pos_pid->p_error = pos_pid->kp * pos_pid->error;
    pos_pid->i_error = pos_pid->integral;
    pos_pid->d_error = pos_pid->kd * (pos_pid->error - pos_pid->error_l);

    pos_pid->last_out = pos_pid->p_error + pos_pid->i_error + pos_pid->d_error;
    if (pos_pid->last_out > pos_pid->maximum)
    {
        pos_pid->last_out = pos_pid->maximum;
    }
    if (pos_pid->last_out < pos_pid->minimum)
    {
        pos_pid->last_out = pos_pid->minimum;
    }

    pos_pid->error_l = pos_pid->error;

    pos_pid->controller.output = pos_pid->last_out;

    return RT_EOK;
}

pos_pid_controller_t pos_pid_controller_create(float kp, float ki, float kd, rt_uint16_t sample_time)
{
    pos_pid_controller_t new_pid = (pos_pid_controller_t)controller_create(sizeof(struct pos_pid_controller), sample_time);
    if(new_pid == RT_NULL)
    {
        return RT_NULL;
    }

    new_pid->kp = kp;
    new_pid->ki = ki;
    new_pid->kd = kd;

    new_pid->maximum = +1000;
    new_pid->minimum = -1000;
    new_pid->anti_windup_value = new_pid->maximum * 0.5f;

    new_pid->integral = 0.0f;
    new_pid->p_error = 0.0f;
    new_pid->i_error = 0.0f;
    new_pid->d_error = 0.0f;

    new_pid->error = 0.0f;
    new_pid->error_l = 0.0f;

    new_pid->last_out = 0.0f;

    new_pid->controller.reset = pos_pid_controller_reset;
    new_pid->controller.destroy = pos_pid_controller_destroy;
    new_pid->controller.update = pos_pid_controller_update;
    new_pid->controller.set_param = pos_pid_controller_set_param;
    new_pid->controller.get_param = pos_pid_controller_get_param;

    return new_pid;
}

rt_err_t pos_pid_controller_set_kp(pos_pid_controller_t pid, float kp)
{
    RT_ASSERT(pid != RT_NULL);

    pid->kp = kp;

    return RT_EOK;
}

rt_err_t pos_pid_controller_set_ki(pos_pid_controller_t pid, float ki)
{
    RT_ASSERT(pid != RT_NULL);

    pid->ki = ki;

    return RT_EOK;
}

rt_err_t pos_pid_controller_set_kd(pos_pid_controller_t pid, float kd)
{
    RT_ASSERT(pid != RT_NULL);

    pid->kd = kd;
    
    return RT_EOK;
}
