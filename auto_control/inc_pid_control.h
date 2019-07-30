#ifndef __INC_PID_CONTROL_H__
#define __INC_PID_CONTROL_H__

#include <rtthread.h>
#include "auto_control.h"

typedef struct inc_pid_control *inc_pid_control_t;

struct inc_pid_control
{
    struct auto_control control;
    
    float     kp;
    float     ki;
    float     kd;

    float     minimum;
    float     maximum;

    float     p_error;
    float     i_error;
    float     d_error;

    float     error;
    float     error_l;
    float     error_ll;
    
    float     last_out;
    rt_tick_t last_time;
};

inc_pid_control_t   inc_pid_control_create(float kp, float ki, float kd);
rt_err_t            inc_pid_control_set_kp(inc_pid_control_t pid, float kp);
rt_err_t            inc_pid_control_set_ki(inc_pid_control_t pid, float ki);
rt_err_t            inc_pid_control_set_kd(inc_pid_control_t pid, float kd);

#endif
