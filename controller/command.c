#include <command.h>

#define DBG_SECTION_NAME  "command"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define TABLE_MAX_SIZE      32

static struct command command_table[TABLE_MAX_SIZE];
static uint8_t command_table_size = 0;

rt_err_t command_register(rt_int8_t cmd, rt_err_t (*handler)(rt_int8_t cmd, void *param))
{
    if (command_table_size > TABLE_MAX_SIZE - 1)
    {
        LOG_E("Command table voerflow");
        return RT_ERROR;
    }
    command_table[command_table_size].robot_cmd = cmd;
    command_table[command_table_size].handler = handler;
    command_table_size += 1;

    return RT_EOK;
}

rt_err_t command_unregister(rt_int8_t cmd)
{
    // TODO

    return RT_EOK;
}

rt_err_t command_handle(rt_int8_t cmd, void *param)
{
    // look-up table and call callback
    for (uint16_t i = 0; i < command_table_size; i++)
    {
        if (command_table[i].robot_cmd == cmd)
        {
            command_table[i].handler(command_table[i].robot_cmd, param);
            return RT_EOK;
        }
    }

    return RT_ERROR;
}

rt_err_t command_send(command_sender_t sender, rt_int8_t cmd, void *param, rt_uint16_t len)
{
    if (sender->send != RT_NULL)
    {
        sender->send(cmd, param, len);
    }
    
    return RT_EOK;
}


