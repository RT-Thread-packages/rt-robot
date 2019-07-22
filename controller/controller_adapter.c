#include <controller_adapter.h>

#define DBG_SECTION_NAME  "adapter"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define MAXIMUN_COMMAND   16

controller_adapter_t controller_adapter_create(rt_mailbox_t contrl_mb, rt_mailbox_t rob_mb)
{
    // Malloc memory for new controller_adapter
    controller_adapter_t new_controller_adapter = (controller_adapter_t) rt_malloc(sizeof(struct controller_adapter));
    if(new_controller_adapter == RT_NULL)
    {
        LOG_E("Falied to allocate memory for controller_adapter\n");
        return RT_NULL;
    }
    new_controller_adapter->table = (struct controller_adapter_table *) rt_malloc(sizeof(struct controller_adapter_table) * MAXIMUN_COMMAND);
    if(new_controller_adapter->table == RT_NULL)
    {
        LOG_E("Falied to allocate memory for controller_adapter table\n");
        return RT_NULL;
    }

    new_controller_adapter->contrl_mb = contrl_mb;
    new_controller_adapter->robot_mb = rob_mb;
    new_controller_adapter->table_max = MAXIMUN_COMMAND;
    new_controller_adapter->table_size = 0;

    return new_controller_adapter;
}

void controller_adapter_destroy(controller_adapter_t adpt)
{
    // LOG_I("Free controller_adapter (type:%d)", controller_adapter->type);

    rt_free(adpt->table);
    rt_free(adpt);
}

rt_err_t controller_adapter_bind_cmd(controller_adapter_t adpt, rt_int8_t contrl_cmd, rt_int8_t rob_cmd)
{
    // TODO
    if (adpt->table_size >= adpt->table_max)
    {
        LOG_E("controller_adapter table is full");
        return RT_ERROR;
    }

    adpt->table[adpt->table_size].ctrl_cmd = contrl_cmd;
    adpt->table[adpt->table_size].rob_cmd = rob_cmd;

    adpt->table_size += 1;

    return RT_EOK;
}

static rt_int8_t controller_adapter_convert_cmd(controller_adapter_t adpt, rt_int8_t contrl_cmd, void *args)
{
    // TODO
    rt_int8_t command = 0;
    // look-up table
    for (int i = 0; i < adpt->table_size; i++)
    {
        if (adpt->table[i].ctrl_cmd == contrl_cmd)
        {
            command = adpt->table[i].rob_cmd;
            break;
        }
    }

    return command;
}

rt_err_t controller_adapter_handle_cmd(controller_adapter_t adpt, rt_int8_t cmd, void *args)
{
    rt_int8_t command = controller_adapter_convert_cmd(adpt, cmd, args);

    adpt->robot_cmd_handler(command, args);

    return RT_EOK;
}

rt_err_t controller_adapter_send_contrl_cmd(controller_adapter_t adpt, rt_int8_t cmd, void *args)
{
    rt_int8_t command = controller_adapter_convert_cmd(adpt, cmd ,args);

    adpt->contrl_cmd_handler(command, args);

    return RT_EOK;
}

// User Application
/*
static void thread_main_entry(void *parameter)
{
    while (1)
    {
        // rt_kprintf("thread1: try to recv a mail\n");

        // 从控制器邮箱中收取邮件
        if (rt_mb_recv(&controller_mb, (rt_uint32_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {

            rt_kprintf("thread1: get a mail from controller, the content:%s\n", str);
            // 原始命令

            // 自动控制 (修改)

            // 控制机器人
            // 转换命令并调用机器人的 cmd_handler
            controller_adapter_handle_cmd(cmd);

        }

        // 从机器人邮箱中收取邮件
        if (rt_mb_recv(&robot_mb, (rt_uint32_t *)&str, RT_WAITING_FOREVER) == RT_EOK)
        {
            // 机器人对手柄的控制
            rt_kprintf("thread1: get a mail from controller, the content:%s\n", str);

            // 控制手柄
            controller_adapter_send_contrl_cmd(cmd);
        }

    }
}
*/
