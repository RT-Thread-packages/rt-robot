#include <controller.h>
#include <ps2_controller.h>

#define DBG_SECTION_NAME  "controller"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define MAXIMUN_COMMAND   16

controller_t controller_create(chassis_t chas, enum controller_type type)
{
    // Malloc memory for new controller
    controller_t new_controller = (controller_t) rt_malloc(sizeof(struct controller));
    if(new_controller == RT_NULL)
    {
        LOG_E("Falied to allocate memory for controller\n");
        return RT_NULL;
    }
    new_controller->table = (struct controller_table *) rt_malloc(sizeof(struct controller_table) * MAXIMUN_COMMAND);
    if(new_controller->table == RT_NULL)
    {
        LOG_E("Falied to allocate memory for controller table\n");
        return RT_NULL;
    }

    new_controller->type = type;
    new_controller->chassis = chas;
    new_controller->table_max = MAXIMUN_COMMAND;
    new_controller->table_size = 0;

    return new_controller;
}

void controller_destroy(controller_t controller)
{
    LOG_I("Free controller (type:%d)", controller->type);

    rt_free(controller->table);
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

rt_err_t controller_bind_command(controller_t controller, rt_int8_t cmd, rt_int8_t chas_cmd)
{
    // TODO
    if (controller->table_size >= controller->table_max)
    {
        LOG_E("Controller table is full");
        return RT_ERROR;
    }

    controller->table[controller->table_size].remote = cmd;
    controller->table[controller->table_size].chassis = chas_cmd;

    controller->table_size += 1;

    return RT_EOK;
}

rt_err_t controller_parse_command(controller_t controller, rt_int8_t cmd, void *args)
{
    // TODO
    rt_int8_t command = 0;

    // look-up table
    for (int i = 0; i < controller->table_size; i++)
    {
        if (controller->table[i].remote == cmd)
        {
            command = controller->table[i].chassis;
            break;
        }
    }

    chassis_parse_command(controller->chassis, command, args);

    return RT_EOK;
}
