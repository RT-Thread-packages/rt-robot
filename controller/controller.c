#include "controller.h"

#define DBG_SECTION_NAME  "controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

// single intput and single output system

controller_t controller_create(rt_size_t size)
{
    // TODO
    // Malloc memory and initialize PID
    controller_t new_controller = (controller_t)rt_malloc(size);
    if(new_controller == RT_NULL)
    {
        LOG_E("Failed to malloc memory for automatic controller\n");
        return RT_NULL;
    }

    new_controller->enable = RT_FALSE;

    return new_controller;
}

rt_err_t controller_update(controller_t controller, float current_point)
{
    RT_ASSERT(controller != RT_NULL);

    if (controller->enable)
    {
        return controller->update(controller, current_point);
    }
    else
    {
        controller->output = controller->target;
    }
    
    return RT_EOK;
}

rt_err_t controller_destroy(controller_t controller)
{
    RT_ASSERT(controller != RT_NULL);
    
    return controller->destroy(controller);
}

rt_err_t controller_enable(controller_t controller)
{
    // TODO
    RT_ASSERT(controller != RT_NULL);

    controller->enable = RT_TRUE;

    return RT_EOK;
}

rt_err_t controller_disable(controller_t controller)
{
    // TODO
    RT_ASSERT(controller != RT_NULL);

    controller->enable = RT_FALSE;

    return RT_EOK;
}

rt_err_t controller_reset(controller_t controller)
{
    // TODO
    RT_ASSERT(controller != RT_NULL);

    return controller->reset(controller);
}

rt_err_t controller_set_target(controller_t controller, rt_int16_t target)
{
    RT_ASSERT(controller != RT_NULL);

    controller->target = target;
    return RT_EOK;
}

rt_err_t controller_set_sample_time(controller_t controller, rt_uint16_t sample_time)
{
    RT_ASSERT(controller != RT_NULL);

    controller->sample_time = sample_time;
    return RT_EOK;
}
