#include <command.h>
#include <chassis.h>

#define DBG_SECTION_NAME  "command"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

// Thread
#define THREAD_PRIORITY             ((RT_THREAD_PRIORITY_MAX / 3) + 1)
#define THREAD_STACK_SIZE           512
#define THREAD_TIMESLICE            10
static rt_thread_t cmd_tid = RT_NULL;

//
struct command_info
{
    rt_uint16_t         cmd;
    void                *target;
};

typedef struct command_info *command_info_t;

// Message Queue
struct command_msg
{
    struct command_info info;
    void                *param;
    rt_uint16_t         size;
};
#define MAX_MSGS                    32
static rt_mq_t cmd_mq = RT_NULL;

static rt_err_t command_handle_get(struct command_msg *msg)
{
    switch (msg->info.cmd)
    {
    case COMMAND_GET_WHEEL0_PID:
        if (msg->size == sizeof(struct cmd_dt_pid))
        {
            struct cmd_dt_pid *ppid = msg->param;
            chassis_t chas = (chassis_t)msg->info.target;
            struct controller_param parameter;

            controller_get_param(chas->c_wheels[0]->w_controller, &parameter);
            ppid->kp = parameter.data.pid.kp;
            ppid->ki = parameter.data.pid.ki;
            ppid->kd = parameter.data.pid.kd;
        }
        else
        {
            return RT_ERROR;
        }
        
        break;
    case COMMAND_GET_WHEEL1_PID:
        if (msg->size == sizeof(struct cmd_dt_pid))
        {
            struct cmd_dt_pid *ppid = msg->param;
            chassis_t chas = (chassis_t)msg->info.target;
            struct controller_param parameter;

            controller_get_param(chas->c_wheels[1]->w_controller, &parameter);
            ppid->kp = parameter.data.pid.kp;
            ppid->ki = parameter.data.pid.ki;
            ppid->kd = parameter.data.pid.kd;
        }
        else
        {
            return RT_ERROR;
        }
        
        break;
    case COMMAND_GET_WHEEL2_PID:
        if (msg->size == sizeof(struct cmd_dt_pid))
        {
            struct cmd_dt_pid *ppid = msg->param;
            chassis_t chas = (chassis_t)msg->info.target;
            struct controller_param parameter;

            controller_get_param(chas->c_wheels[2]->w_controller, &parameter);
            ppid->kp = parameter.data.pid.kp;
            ppid->ki = parameter.data.pid.ki;
            ppid->kd = parameter.data.pid.kd;
        }
        else
        {
            return RT_ERROR;
        }
        
        break;
    case COMMAND_GET_WHEEL3_PID:
        if (msg->size == sizeof(struct cmd_dt_pid))
        {
            struct cmd_dt_pid *ppid = msg->param;
            chassis_t chas = (chassis_t)msg->info.target;
            struct controller_param parameter;

            controller_get_param(chas->c_wheels[3]->w_controller, &parameter);
            ppid->kp = parameter.data.pid.kp;
            ppid->ki = parameter.data.pid.ki;
            ppid->kd = parameter.data.pid.kd;
        }
        else
        {
            return RT_ERROR;
        }
        
        break;
    case COMMAND_GET_WHEELS_PID:
        if (msg->size >= 2 * sizeof(struct cmd_dt_pid))
        {
            struct cmd_dt_pid *ppid = msg->param;
            chassis_t chas = (chassis_t)msg->info.target; 
            struct controller_param parameter;

            for (int i = 0; (i < chas->c_kinematics->total_wheels) && (i < (msg->size / sizeof(struct cmd_dt_pid))); i++)
            {
                controller_get_param(chas->c_wheels[i]->w_controller, &parameter);
                ppid[i].id = COMMAND_GET_WHEEL0_PID + i;
                ppid[i].kp = parameter.data.pid.kp;
                ppid[i].ki = parameter.data.pid.ki;
                ppid[i].kd = parameter.data.pid.kd;
            }
        }
        else
        {
            return RT_ERROR;
        }
        
        break;
    
    default: return RT_ERROR;
    }

    return RT_EOK;
}

rt_err_t command_handle(rt_uint16_t cmd, void *param, rt_uint16_t size, void *target)
{
    struct command_msg msg = {
        .param = param,
        .size = size,
        .info = {
            .target = target,
            .cmd = cmd,
        }
    };

    if (cmd > COMMAND_GET_START && cmd < COMMAND_GET_END)
    {
        return command_handle_get(&msg);
    }

    return rt_mq_send(cmd_mq, &msg, sizeof(struct command_msg));
}

rt_err_t command_send(command_sender_t sender, rt_int16_t cmd, void *param, rt_uint16_t size)
{
    if (sender->send != RT_NULL)
    {
        return sender->send(cmd, param, size);
    }

    return RT_ERROR;
}

static void command_thread_entry(void *param)
{
    struct command_msg msg;

    while (1)
    {
        rt_mq_recv(cmd_mq, &msg, sizeof(struct command_msg), RT_WAITING_FOREVER);

        // TODO
        switch (msg.info.cmd)
        {
        case COMMAND_SET_WHEELS_PID:
            if (msg.size >= 2 * sizeof(struct cmd_dt_pid))
            {
                chassis_t chas = (chassis_t)msg.info.target; 
                struct cmd_dt_pid *ppid = msg.param;
                struct controller_param parameter;
                for (int i = 0; i < chas->c_kinematics->total_wheels; i++)
                {
                    parameter.data.pid.kp = ppid[i].kp;
                    parameter.data.pid.ki = ppid[i].ki;
                    parameter.data.pid.kd = ppid[i].kd;
                    controller_set_param(chas->c_wheels[ppid[i].id]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_WHEEL0_PID:
            if (msg.size == sizeof(struct cmd_dt_pid))
            {
                chassis_t chas = (chassis_t)msg.info.target; 
                if (chas->c_kinematics->total_wheels > 0)
                {
                    struct cmd_dt_pid *ppid = msg.param;
                    struct controller_param parameter = {
                        .data.pid.kp = ppid->kp,
                        .data.pid.ki = ppid->ki,
                        .data.pid.kd = ppid->kd
                    };
                    controller_set_param(chas->c_wheels[0]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_WHEEL1_PID:
            if (msg.size == sizeof(struct cmd_dt_pid))
            {
                chassis_t chas = (chassis_t)msg.info.target; 
                if (chas->c_kinematics->total_wheels > 1)
                {
                    struct cmd_dt_pid *ppid = msg.param;
                    struct controller_param parameter = {
                        .data.pid.kp = ppid->kp,
                        .data.pid.ki = ppid->ki,
                        .data.pid.kd = ppid->kd
                    };
                    controller_set_param(chas->c_wheels[1]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_WHEEL2_PID:
            if (msg.size == sizeof(struct cmd_dt_pid))
            {
                chassis_t chas = (chassis_t)msg.info.target; 
                if (chas->c_kinematics->total_wheels > 2)
                {
                    struct cmd_dt_pid *ppid = msg.param;
                    struct controller_param parameter = {
                        .data.pid.kp = ppid->kp,
                        .data.pid.ki = ppid->ki,
                        .data.pid.kd = ppid->kd
                    };
                    controller_set_param(chas->c_wheels[2]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_WHEEL3_PID:
            if (msg.size == sizeof(struct cmd_dt_pid))
            {
                chassis_t chas = (chassis_t)msg.info.target; 
                if (chas->c_kinematics->total_wheels > 3)
                {
                    struct cmd_dt_pid *ppid = msg.param;
                    struct controller_param parameter = {
                        .data.pid.kp = ppid->kp,
                        .data.pid.ki = ppid->ki,
                        .data.pid.kd = ppid->kd
                    };
                    controller_set_param(chas->c_wheels[3]->w_controller, &parameter);
                }
            }
            break;
        default:
            break;
        }
    }
}

void command_init(void)
{
    cmd_mq = rt_mq_create("command", sizeof(struct command_info), MAX_MSGS, RT_IPC_FLAG_FIFO);
    if (cmd_mq == RT_NULL)
    {
        LOG_E("Failed to creat meassage queue");
        return;
    }

    cmd_tid = rt_thread_create("command",
                              command_thread_entry, RT_NULL,
                              THREAD_STACK_SIZE,
                              THREAD_PRIORITY, THREAD_TIMESLICE);

    if (cmd_tid == RT_NULL)
    {
        LOG_E("Failed to creat thread");
        return;
    }
    rt_thread_startup(cmd_tid);
}
