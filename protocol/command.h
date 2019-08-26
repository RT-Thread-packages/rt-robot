/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <rtthread.h>
#include <chassis.h>

/* Command */
// down-link
#define COMMAND_NONE                                    (rt_uint16_t)(0X0000)
#define COMMAND_SET_CHASSIS_STOP                        (rt_uint16_t)(0X0001)
#define COMMAND_SET_CHASSIS_FORWARD                     (rt_uint16_t)(0X0002)
#define COMMAND_SET_CHASSIS_BACKWARD                    (rt_uint16_t)(0X0003)
#define COMMAND_SET_CHASSIS_ROTATE_LEFT                 (rt_uint16_t)(0X0004)
#define COMMAND_SET_CHASSIS_ROTATE_RIGHT                (rt_uint16_t)(0X0005)
#define COMMAND_SET_CHASSIS_MOVE_LEFT                   (rt_uint16_t)(0X0006)
#define COMMAND_SET_CHASSIS_MOVE_RIGHT                  (rt_uint16_t)(0X0007)

#define COMMAND_SET_CHASSIS_FORWARD_WITH_PARAM          (rt_uint16_t)(0X1002)
#define COMMAND_SET_CHASSIS_BACKWARD_WITH_PARAM         (rt_uint16_t)(0X1003)
#define COMMAND_SET_CHASSIS_ROTATE_LEFT_WITH_PARAM      (rt_uint16_t)(0X1004)
#define COMMAND_SET_CHASSIS_ROTATE_RIGHT_WITH_PARAM     (rt_uint16_t)(0X1005)
#define COMMAND_SET_CHASSIS_MOVE_LEFT_WITH_PARAM        (rt_uint16_t)(0X1006)
#define COMMAND_SET_CHASSIS_MOVE_RIGHT_WITH_PARAM       (rt_uint16_t)(0X1007)
#define COMMAND_SET_CHASSIS_VELOCITY_LINEAR_X           (rt_uint16_t)(0x1008)
#define COMMAND_SET_CHASSIS_VELOCITY_LINEAR_Y           (rt_uint16_t)(0x1009)
#define COMMAND_SET_CHASSIS_VELOCITY_ANGULAR_Z          (rt_uint16_t)(0x100A)

#define COMMAND_SET_WHEEL0_PID                          (rt_uint16_t)(0x2000)
#define COMMAND_SET_WHEEL1_PID                          (rt_uint16_t)(0x2001)
#define COMMAND_SET_WHEEL2_PID                          (rt_uint16_t)(0x2002)
#define COMMAND_SET_WHEEL3_PID                          (rt_uint16_t)(0x2003)
#define COMMAND_SET_WHEELS_PID                          (rt_uint16_t)(0x2004)
#define COMMAND_SET_DEFAULT_PID                         (rt_uint16_t)(0x3000)

// ! Must keep all COMMAND_GET_XXX value is between COMMAND_GET_START and COMMAND_GET_END
#define COMMAND_GET_START                               (rt_uint16_t)(0x5000)
#define COMMAND_GET_WHEEL0_PID                          (rt_uint16_t)(0x5001)
#define COMMAND_GET_WHEEL1_PID                          (rt_uint16_t)(0x5002)
#define COMMAND_GET_WHEEL2_PID                          (rt_uint16_t)(0x5003)
#define COMMAND_GET_WHEEL3_PID                          (rt_uint16_t)(0x5004)
#define COMMAND_GET_WHEELS_PID                          (rt_uint16_t)(0x5005)
#define COMMAND_GET_END                                 (rt_uint16_t)(0x8FFF)

// up-link
#define COMMAND_RC_VIBRATE                              (rt_uint16_t)(0x9000)
#define COMMAND_SEND_PID                                (rt_uint16_t)(0xA000)
#define COMMAND_SEND_SENSOR                             (rt_uint16_t)(0xA001)
#define COMMAND_SEND_RPY                                (rt_uint16_t)(0xA002)

// PID ID
#define PID_ID_WHEEL_0                          1
#define PID_ID_WHEEL_1                          2
#define PID_ID_WHEEL_2                          3
#define PID_ID_WHEEL_3                          4

struct cmd_pid
{
    int   id;       // range: 1 ~ max
    float kp;
    float ki;
    float kd;
};

struct cmd_sensor
{
    int32_t acc_x;
    int32_t acc_y;
    int32_t acc_z;
    int32_t gyro_x;
    int32_t gyro_y;
    int32_t gyro_z;
    int32_t mag_x;
    int32_t mag_y;
    int32_t mag_z;
};

struct cmd_rpy
{
    float roll;
    float pitch;
    float yaw;
};

struct cmd_velocity
{
    union
    {
        float linear_x;     // m/s
        float linear_y;
        float angular_z;
        float common;
    } data; 
};

struct command_sender
{
    char *name;
    rt_err_t (*send)(rt_uint16_t cmd, void *param, rt_uint16_t size);
};

typedef struct command_sender *command_sender_t;

rt_err_t command_handle(rt_uint16_t cmd, void *param, rt_uint16_t size);
rt_err_t command_send(command_sender_t sender, rt_uint16_t cmd, void *param, rt_uint16_t size);
rt_err_t command_init(chassis_t chas);

#endif
