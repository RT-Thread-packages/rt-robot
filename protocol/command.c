/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include <command.h>

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

// Target
chassis_t g_chas;

static rt_err_t command_handle_get(struct command_msg *msg)
{
    switch (msg->info.cmd)
    {
    case COMMAND_GET_WHEEL0_PID:
    case COMMAND_GET_WHEEL1_PID:
    case COMMAND_GET_WHEEL2_PID:
    case COMMAND_GET_WHEEL3_PID:
        if (msg->size == sizeof(struct cmd_pid))
        {
            struct cmd_pid *ppid = msg->param;
            struct controller_param parameter;

            controller_get_param(g_chas->c_wheels[msg->info.cmd - COMMAND_GET_WHEEL0_PID]->w_controller, &parameter);
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
        if (msg->size >= 2 * sizeof(struct cmd_pid))
        {
            struct cmd_pid *ppid = msg->param;
            struct controller_param parameter;

            for (int i = 0; (i < g_chas->c_kinematics->total_wheels) && (i < (msg->size / sizeof(struct cmd_pid))); i++)
            {
                controller_get_param(g_chas->c_wheels[i]->w_controller, &parameter);
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

rt_err_t command_handle(rt_uint16_t cmd, void *param, rt_uint16_t size)
{
    struct command_msg msg = {
        .param = param,
        .size = size,
        .info = {
            .cmd = cmd,
        }
    };

    if (cmd > COMMAND_GET_START && cmd < COMMAND_GET_END)
    {
        return command_handle_get(&msg);
    }

    return rt_mq_send(cmd_mq, &msg, sizeof(struct command_msg));
}

rt_err_t command_send(command_sender_t sender, rt_uint16_t cmd, void *param, rt_uint16_t size)
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

        if (g_chas == RT_NULL)
        {
            LOG_D("The target is NULL");
            continue;
        }

        switch (msg.info.cmd)
        {
        case COMMAND_SET_WHEELS_PID:
            if (msg.size >= 2 * sizeof(struct cmd_pid))
            {
                struct cmd_pid *ppid = msg.param;
                struct controller_param parameter;
                for (int i = 0; i < g_chas->c_kinematics->total_wheels; i++)
                {
                    parameter.data.pid.kp = ppid[i].kp;
                    parameter.data.pid.ki = ppid[i].ki;
                    parameter.data.pid.kd = ppid[i].kd;
                    controller_set_param(g_chas->c_wheels[ppid[i].id]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_WHEEL0_PID:
        case COMMAND_SET_WHEEL1_PID:
        case COMMAND_SET_WHEEL2_PID:
        case COMMAND_SET_WHEEL3_PID:
            if (msg.size == sizeof(struct cmd_pid))
            {
                if (g_chas->c_kinematics->total_wheels > msg.info.cmd - COMMAND_SET_WHEEL0_PID)
                {
                    struct cmd_pid *ppid = msg.param;
                    struct controller_param parameter = {
                        .data.pid.kp = ppid->kp,
                        .data.pid.ki = ppid->ki,
                        .data.pid.kd = ppid->kd
                    };
                    controller_set_param(g_chas->c_wheels[msg.info.cmd - COMMAND_SET_WHEEL0_PID]->w_controller, &parameter);
                }
            }
            break;
        case COMMAND_SET_CHASSIS_STOP:
            chassis_move(g_chas, 0.0f);
            break;
        case COMMAND_SET_CHASSIS_FORWARD:
            chassis_straight(g_chas, CHASSIS_VELOCITY_LINEAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_BACKWARD:
            chassis_straight(g_chas, -CHASSIS_VELOCITY_LINEAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_ROTATE_LEFT:
            chassis_rotate(g_chas, CHASSIS_VELOCITY_ANGULAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_ROTATE_RIGHT:
            chassis_rotate(g_chas, -CHASSIS_VELOCITY_ANGULAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_MOVE_LEFT:
            chassis_move(g_chas, CHASSIS_VELOCITY_LINEAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_MOVE_RIGHT:
            chassis_move(g_chas, -CHASSIS_VELOCITY_LINEAR_MAXIMUM / 2.0f);
            break;
        case COMMAND_SET_CHASSIS_FORWARD_WITH_PARAM:
        case COMMAND_SET_CHASSIS_BACKWARD_WITH_PARAM:
        case COMMAND_SET_CHASSIS_ROTATE_LEFT_WITH_PARAM:
        case COMMAND_SET_CHASSIS_ROTATE_RIGHT_WITH_PARAM:
        case COMMAND_SET_CHASSIS_MOVE_LEFT_WITH_PARAM:
        case COMMAND_SET_CHASSIS_MOVE_RIGHT_WITH_PARAM:
            if (msg.size == sizeof(struct cmd_velocity))
            {
                struct cmd_velocity *target_velocity = (struct cmd_velocity *)msg.param;
                rt_err_t (*p_fun)(chassis_t chas, float data);
                float tmp;
                if (msg.info.cmd == COMMAND_SET_CHASSIS_FORWARD_WITH_PARAM)
                {
                    tmp = target_velocity->data.linear_x;
                    p_fun = chassis_straight;
                }
                else if (msg.info.cmd == COMMAND_SET_CHASSIS_BACKWARD_WITH_PARAM)
                {
                    tmp = -target_velocity->data.linear_x;
                    p_fun = chassis_straight;
                }
                else if (msg.info.cmd == COMMAND_SET_CHASSIS_ROTATE_LEFT_WITH_PARAM)
                {
                    tmp = target_velocity->data.angular_z;
                    p_fun = chassis_rotate;
                }
                else if (msg.info.cmd == COMMAND_SET_CHASSIS_ROTATE_RIGHT_WITH_PARAM)
                {
                    tmp = -target_velocity->data.angular_z;
                    p_fun = chassis_rotate;
                }
                else if (msg.info.cmd == COMMAND_SET_CHASSIS_MOVE_LEFT_WITH_PARAM)
                {
                    tmp = target_velocity->data.linear_y;
                    p_fun = chassis_move;
                }
                else
                {
                    tmp = -target_velocity->data.linear_y;
                    p_fun = chassis_move;
                }
                
                p_fun(g_chas, tmp);
            }
            break;
        case COMMAND_SET_CHASSIS_VELOCITY_LINEAR_X:
        case COMMAND_SET_CHASSIS_VELOCITY_LINEAR_Y:
        case COMMAND_SET_CHASSIS_VELOCITY_ANGULAR_Z:
            if (msg.size == sizeof(struct cmd_velocity))
            {
                struct cmd_velocity *target_velocity = (struct cmd_velocity *)msg.param;
                rt_err_t (*p_fun)(chassis_t chas, float data);
                float tmp;
                if (msg.info.cmd == COMMAND_SET_CHASSIS_VELOCITY_LINEAR_X)
                {
                    tmp = target_velocity->data.linear_x;
                    p_fun = chassis_set_velocity_x;
                }
                else if (msg.info.cmd == COMMAND_SET_CHASSIS_VELOCITY_LINEAR_Y)
                {
                    tmp = target_velocity->data.linear_y;
                    p_fun = chassis_set_velocity_y;
                }
                else
                {
                    tmp = target_velocity->data.angular_z;
                    p_fun = chassis_set_velocity_z;
                }

                p_fun(g_chas, tmp);
            }
            break;
        default:
            break;
        }
    }
}

rt_err_t command_init(chassis_t chas)
{
    cmd_mq = rt_mq_create("command", sizeof(struct command_msg), MAX_MSGS, RT_IPC_FLAG_FIFO);
    if (cmd_mq == RT_NULL)
    {
        LOG_E("Failed to creat meassage queue");
        return RT_ERROR;
    }

    cmd_tid = rt_thread_create("command",
                              command_thread_entry, RT_NULL,
                              THREAD_STACK_SIZE,
                              THREAD_PRIORITY, THREAD_TIMESLICE);

    if (cmd_tid == RT_NULL)
    {
        LOG_E("Failed to creat thread");
        return RT_ERROR;
    }

    g_chas = chas;

    rt_thread_startup(cmd_tid);

    return RT_EOK;
}
