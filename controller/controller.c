#include "controller.h"

#define DBG_SECTION_NAME  "controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

controller_t controller_create(void)
{
    // 1. Malloc memory
    controller_t new_controller = (controller_t)rt_malloc(sizeof(struct controller));
    if(new_controller == RT_NULL)
    {
        LOG_E("Falied to allocate memory for controller");
        return RT_NULL;
    }

    return new_controller;
}

void controller_destroy(controller_t contrl)
{

}

void controller_enable(void)
{

}

void controller_disable(void)
{

}
