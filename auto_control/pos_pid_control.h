#ifndef __POS_PID_CONTROL_H__
#define __POS_PID_CONTROL_H__

#include <rtthread.h>
#include "auto_control.h"

typedef struct pos_pid_control *pos_pid_control_t;

struct pos_pid_control
{
    struct auto_control control;
    
    float     kp;
    float     ki;
    float     kd;

    float     minimum;
    float     maximum;
    float     anti_windup_value;

    float     p_error;
    float     i_error;
    float     d_error;

    float     integral;
    float     error;
    float     error_l;
    
    float     last_out;
    rt_tick_t last_time;
};

pos_pid_control_t   pos_pid_control_create(float kp, float ki, float kd);
rt_err_t            pos_pid_control_set_kp(pos_pid_control_t pid, float kp);
rt_err_t            pos_pid_control_set_ki(pos_pid_control_t pid, float ki);
rt_err_t            pos_pid_control_set_kd(pos_pid_control_t pid, float kd);

#endif
