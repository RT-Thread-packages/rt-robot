#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <rtthread.h>

struct controller
{
    float       target;
    float       output;
    rt_uint16_t sample_time;    // unit:ms
    int         enable;

    rt_err_t    (*update)(void *controller, float current_point);
    rt_err_t    (*reset)(void *controller);
    rt_err_t    (*destroy)(void *controller);
};

typedef struct controller *controller_t;

controller_t    controller_create(rt_size_t size, rt_uint16_t sample_time);
rt_err_t        controller_update(controller_t controller, float current_point);
rt_err_t        controller_destroy(controller_t controller);
rt_err_t        controller_enable(controller_t controller);
rt_err_t        controller_disable(controller_t controller);
rt_err_t        controller_reset(controller_t controller);
rt_err_t        controller_set_target(controller_t controller, rt_int16_t target);
rt_err_t        controller_set_sample_time(controller_t controller, rt_uint16_t sample_time);

#endif // __CONTROLLER_H__
