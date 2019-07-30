#ifndef __auto_control_H__
#define __auto_control_H__

#include <rtthread.h>

struct auto_control
{
    float       target;
    float       output;
    rt_uint16_t sample_time;    // unit:ms
    int         enable;

    rt_err_t    (*update)(void *control, float current_point);
    rt_err_t    (*reset)(void *control);
    rt_err_t    (*destroy)(void *control);
};

typedef struct auto_control *auto_control_t;

auto_control_t  auto_control_create(rt_size_t size);
rt_err_t        auto_control_update(auto_control_t control, float current_point);
rt_err_t        auto_control_destroy(auto_control_t control);
rt_err_t        auto_control_enable(auto_control_t control);
rt_err_t        auto_control_disable(auto_control_t control);
rt_err_t        auto_control_reset(auto_control_t control);
rt_err_t        auto_control_set_target(auto_control_t control, rt_int16_t target);
rt_err_t        auto_control_set_sample_time(auto_control_t control, rt_uint16_t sample_time);

#endif // __auto_control_H__
