#include <controller.h>
#include <ps2_controller.h>

#define DBG_SECTION_NAME  "controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

controller_t controller_create(enum controller_type type)
{
    // Malloc memory for new controller

    if (type == PS2)
    {
        ps2_init();
    }
    else
    {
        LOG_E("Unknown controller type\n");
        return RT_NULL;
    }

    controller_t new_controller = (controller_t) rt_malloc(sizeof(struct controller));
    if(new_controller == RT_NULL)
    {
        LOG_E("Falied to allocate memory for chassis\n");
        return RT_NULL;
    }

    new_controller->type = type;

    return new_controller;
}

void controller_destroy(controller_t controller)
{
    LOG_I("Free controller (type:%d)", controller->type);

    rt_free(controller);
}

rt_err_t controller_enable(controller_t controller)
{
    // TODO
    
    return RT_EOK;
}

rt_err_t controller_disable(controller_t controller)
{
    // TODO
    
    return RT_EOK;
}

rt_err_t controller_get(controller_t controller)
{
    // TODO

    if (controller->type == PS2)
    {
        ps2_ctrl_data_t ps2data;

        if (ps2_scan(&ps2data))
        {
            controller->digital = 0;
            if (!(ps2data.button & PS2_BUTTON_START))
            {
                controller->digital |= CONTROLLER_BITMASK_START;
            }
            if (!(ps2data.button & PS2_BUTTON_SELECT))
            {
                controller->digital |= CONTROLLER_BITMASK_STOP;
            }
            controller->analog.x = -(100 * (ps2data.left_stick_y - 0x7F) / 127);
            controller->analog.y = +(100 * (ps2data.left_stick_x - 0x80) / 127);
            controller->analog.z = -(100 * (ps2data.right_stick_x - 0x80) / 127);
        }
        else
        {
            return RT_ERROR;
        }
        
        return RT_EOK;
    }

    return RT_ERROR;
}

rt_err_t controller_set(controller_t controller, int cmd, void *arg)
{
    // TODO
    
    if (controller->type == PS2)
    {
        if (cmd == CONTROLLER_CMD_REMIND)
        {
            ps2_enter_config();
            ps2_open_vibration_mode();
            ps2_exit_config();
            ps2_vibrating(0, 70);
            rt_thread_mdelay(1000);
            ps2_vibrating(0, 0);
        }

        return RT_EOK;
    }

    return RT_ERROR;
}
