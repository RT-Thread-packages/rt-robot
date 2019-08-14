#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <rtthread.h>

/* Command */
// down-link
#define COMMAND_NONE                            0X0000
#define COMMAND_CAR_STOP                        0X0001
#define COMMAND_CAR_FORWARD                     0X0002
#define COMMAND_CAR_BACKWARD                    0X0003
#define COMMAND_CAR_TURNLEFT                    0X0004
#define COMMAND_CAR_TURNRIGHT                   0X0005

#define COMMAND_CAR_FORWARD_WITH_PARAM          0X1002
#define COMMAND_CAR_BACKWARD_WITH_PARAM         0X1003
#define COMMAND_CAR_TURNLEFT_WITH_PARAM         0X1004
#define COMMAND_CAR_TURNRIGHT_WITH_PARAM        0X1005
#define COMMAND_SET_CAR_VELOCITY_LINEAR_X       0x1006
#define COMMAND_SET_CAR_VELOCITY_LINEAR_Y       0x1007
#define COMMAND_SET_CAR_VELOCITY_ANGULAR_Z      0x1008

#define COMMAND_SET_PID                         0x3000
#define COMMAND_RESET_PID                       0x4000
#define COMMAND_REQUEST_PID                     0x5000

// up-link
#define COMMAND_RC_VIBRATE                      0x2000

#define COMMAND_SEND_PID                        0x6000
#define COMMAND_SEND_SENSOR                     0x6001
#define COMMAND_SEND_RPY                        0x6002

// PID ID
#define PID_ID_WHEEL_0                          1
#define PID_ID_WHEEL_1                          2
#define PID_ID_WHEEL_2                          3
#define PID_ID_WHEEL_3                          4

struct cmd_dt_pid
{
    int   id;       // range: 1 ~ max
    float kp;
    float ki;
    float kd;
};

struct cmd_dt_sensor
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

struct cmd_dt_rpy
{
    float roll;
    float pitch;
    float yaw;
};

struct cmd_dt_velocity
{
    union
    {
        float linear_x;     // m/s
        float linear_y;
        float angular_z;
    } data; 
};

struct command_sender
{
    char *name;
    rt_err_t (*send)(rt_int16_t cmd, void *param, rt_uint16_t size);
};

typedef struct command_sender *command_sender_t;

struct command_info
{
    rt_int16_t  cmd;
    void        *target;
};

typedef struct command_info *command_info_t;

rt_err_t command_register(rt_int16_t cmd, void (*handler)(command_info_t info, void *param, rt_uint16_t size));
rt_err_t command_unregister(rt_int16_t cmd);
rt_err_t command_handle(rt_int16_t cmd, void *param, rt_uint16_t size, void *target);
rt_err_t command_send(command_sender_t sender, rt_int16_t cmd, void *param, rt_uint16_t size);

#endif
