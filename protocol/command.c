#include <command.h>

#define DBG_SECTION_NAME  "command"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

// Thread
#define THREAD_PRIORITY             ((RT_THREAD_PRIORITY_MAX / 3) + 1)
#define THREAD_STACK_SIZE           512
#define THREAD_TIMESLICE            10
static rt_thread_t trd_cmd = RT_NULL;

// Message Queue
struct command_msg
{
    struct command_info info;
    void                *param;
    rt_uint16_t         size;
};
#define MAX_MSGS                    16
static rt_mq_t msg_cmd = RT_NULL;

// Table
#define TABLE_MAX_SIZE              32
struct command
{
    rt_int16_t   robot_cmd;
    void         (*handler)(command_info_t info, void *param, rt_uint16_t size);
};
static struct command command_table[TABLE_MAX_SIZE];
static uint16_t command_table_size = 0;

rt_err_t command_register(rt_int16_t cmd, void (*handler)(command_info_t info, void *param, rt_uint16_t size))
{
    if (command_table_size > TABLE_MAX_SIZE - 1)
    {
        LOG_E("Command table voerflow");
        return RT_ERROR;
    }
    command_table[command_table_size].robot_cmd = cmd;
    command_table[command_table_size].handler = handler;
    command_table_size++;

    return RT_EOK;
}

rt_err_t command_unregister(rt_int16_t cmd)
{
    for (int i = 0; i < command_table_size; i++)
    {
        if (command_table[i].robot_cmd == cmd)
        {
            for (int j = i; j < command_table_size - 1; j++)
            {
                rt_memcpy(&command_table[j], &command_table[j+1], sizeof(struct command));
            }
            command_table_size--;
            break;
        }
    }

    return RT_EOK;
}

rt_err_t command_handle(rt_int16_t cmd, void *param, rt_uint16_t size, void *target)
{
    struct command_msg msg = {
        .param = param,
        .size = size,
        .info = {
            .target = target,
            .cmd = cmd,
        }
    };
    return rt_mq_send(msg_cmd, &msg, sizeof(struct command_msg));
}

rt_err_t command_send(command_sender_t sender, rt_int16_t cmd, void *param, rt_uint16_t size)
{
    if (sender->send != RT_NULL)
    {
        return sender->send(cmd, param, size);
    }

    return RT_EOK;
}

static void command_thread_entry(void *param)
{
    struct command_msg msg;

    while (1)
    {
        rt_mq_recv(msg_cmd, &msg, sizeof(struct command_msg), RT_WAITING_FOREVER);
        // look-up table and call callback
        for (int i = 0; i < command_table_size; i++)
        {
            if (command_table[i].robot_cmd == msg.info.cmd)
            {
                command_table[i].handler(&msg.info, msg.param, msg.size);
                break;
            }
        }
    }
}

void command_init(void)
{
    msg_cmd = rt_mq_create("command", sizeof(struct command_info), MAX_MSGS, RT_IPC_FLAG_FIFO);
    if (msg_cmd == RT_NULL)
    {
        LOG_E("Failed to creat meassage queue");
        return;
    }

    trd_cmd = rt_thread_create("command",
                              command_thread_entry, RT_NULL,
                              THREAD_STACK_SIZE,
                              THREAD_PRIORITY, THREAD_TIMESLICE);

    if (trd_cmd == RT_NULL)
    {
        LOG_E("Failed to creat thread");
        return;
    }
    rt_thread_startup(trd_cmd);
}
