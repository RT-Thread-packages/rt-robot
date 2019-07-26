#include "pid.h"

#define DBG_SECTION_NAME  "pid"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

pid_control_t pid_create(void)
{
    // TODO
    // Malloc memory and initialize PID
    pid_control_t new_pid = (pid_control_t)rt_malloc(sizeof(struct pid_control));
    if(new_pid == RT_NULL)
    {
        LOG_E("Failed to malloc memory for PID\n");
    }

    pid_set_kp(new_pid, 0.05f);
    pid_set_ki(new_pid, 0.01f);
    pid_set_kd(new_pid, 0.02f);

    new_pid->maximum = 100;
    new_pid->minimum = 20;
    new_pid->anti_windup_value = new_pid->maximum * 2.0f;

    new_pid->p_error = 10.0f;
    new_pid->i_error = 1.0f;
    new_pid->d_error = 0.2f;

    new_pid->integral = 0.0f;
    new_pid->error = 0.0f;
    new_pid->error_l = 0.0f;
    new_pid->error_ll = 0.0f;

    new_pid->out = 0.0f;
    new_pid->last_out = 0.0f;

    return new_pid;
}

void pid_destroy(pid_control_t pid)
{
    LOG_D("Free PID");
    rt_free(pid);
}

rt_err_t pid_reset(pid_control_t pid)
{
    // TODO
    rt_memset(pid, 0, sizeof(pid_control_t));
    return RT_EOK;
}

rt_err_t pid_set_kp(pid_control_t pid, float kp)
{
    pid->kp = kp;
    return RT_EOK;
}

rt_err_t pid_set_ki(pid_control_t pid, float ki)
{
    pid->ki = ki;
    return RT_EOK;
}

rt_err_t pid_set_kd(pid_control_t pid, float kd)
{
    pid->kd = kd;
    return RT_EOK;
}

rt_err_t pid_set_target(pid_control_t pid, rt_int16_t set_point)
{
    // TODO
    pid->set_point = set_point;

    return RT_EOK;
}

rt_err_t pid_set_anti_windup_value(pid_control_t pid, float anti_windup_value)
{
    pid->anti_windup_value = anti_windup_value;
    return RT_EOK;
}

rt_err_t pid_set_sample_time(pid_control_t pid, rt_uint16_t sample_time)
{
    // TODO
    pid->sample_time = sample_time;
    return RT_EOK;
}

float pid_update(pid_control_t pid, rt_int16_t current_point)
{
    // TODO
    if((rt_tick_get() - pid->last_time) < rt_tick_from_millisecond(pid->sample_time))
    {
        LOG_D("PID waiting ... ");
        return pid->last_out;
    }

    pid->last_time = rt_tick_get();
    pid->error = pid->set_point - current_point;

    pid->integral += pid->error;

    //Perform integral value capping to avoid internal PID state to blows up
    //when actuators saturate:
    if(pid->integral > pid->anti_windup_value) {
        pid->integral = pid->anti_windup_value;
    } else if (pid->integral < -pid->anti_windup_value) {
        pid->integral = -pid->anti_windup_value;
    }

    pid->p_error = pid->kp * pid->error;
    pid->i_error = pid->ki * pid->integral;
    pid->d_error = pid->kd * (pid->error - 2 * pid->error_l + pid->error_ll);

    pid->out = pid->p_error + pid->i_error + pid->d_error;
    if (pid->out > pid->maximum)
    {
        pid->out = pid->maximum;
    }
    if (pid->out < pid->minimum)
    {
        pid->out = pid->minimum;
    }

    pid->last_out = pid->out;
    pid->error_ll = pid->error_l;
    pid->error_l = pid->error;

    rt_kprintf("%d - %d\n", current_point, pid->set_point);
    //LOG_D("PID current: %d : setpoint %d - P%d I%d D%d - [%d]", current_point, pid->set_point, (int)(pid->p_error + 0.5f), (int)(pid->i_error + 0.5f), (int)(pid->d_error + 0.5f), (int)(pid->out + 0.5f));
    // LOG_D("PID P Error: %d", (int)(pid->p_error + 0.5f));
    // LOG_D("PID I Error: %d", (int)(pid->i_error + 0.5f));
    // LOG_D("PID D Error: %d", (int)(pid->d_error + 0.5f));
    // LOG_D("PID Last Out: %d", (int)(pid->last_out + 0.5f));
    // LOG_D("PID Out: %d", (int)(pid->out + 0.5f));

    return pid->out;
}

// PID Thread
