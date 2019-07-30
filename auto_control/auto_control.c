#include "auto_control.h"

#define DBG_SECTION_NAME  "auto_control_pid"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

// single intput and single output system

auto_control_t auto_control_create(rt_size_t size)
{
    // TODO
    // Malloc memory and initialize PID
    auto_control_t new_control = (auto_control_t)rt_malloc(size);
    if(new_control == RT_NULL)
    {
        LOG_E("Failed to malloc memory for automatic control\n");
        return RT_NULL;
    }

    new_control->enable = RT_FALSE;

    return new_control;
}

rt_err_t auto_control_update(auto_control_t control, float current_point)
{
    RT_ASSERT(control != RT_NULL);

    if (control->enable)
    {
        return control->update(control, current_point);
    }
    else
    {
        control->output = control->target;
    }
    
    return RT_EOK;
}

rt_err_t auto_control_destroy(auto_control_t control)
{
    RT_ASSERT(control != RT_NULL);
    
    return control->destroy(control);
}

rt_err_t auto_control_enable(auto_control_t control)
{
    // TODO
    RT_ASSERT(control != RT_NULL);

    control->enable = RT_TRUE;

    return RT_EOK;
}

rt_err_t auto_control_disable(auto_control_t control)
{
    // TODO
    RT_ASSERT(control != RT_NULL);

    control->enable = RT_FALSE;

    return RT_EOK;
}

rt_err_t auto_control_reset(auto_control_t control)
{
    // TODO
    RT_ASSERT(control != RT_NULL);

    return control->reset(control);
}

rt_err_t auto_control_set_target(auto_control_t control, rt_int16_t target)
{
    RT_ASSERT(control != RT_NULL);

    control->target = target;
    return RT_EOK;
}

rt_err_t auto_control_set_sample_time(auto_control_t control, rt_uint16_t sample_time)
{
    RT_ASSERT(control != RT_NULL);

    control->sample_time = sample_time;
    return RT_EOK;
}
