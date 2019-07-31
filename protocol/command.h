#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <rtthread.h>

// command
#define COMMAND_NONE                    0
#define COMMAND_CAR_STOP                1
#define COMMAND_CAR_FORWARD             2
#define COMMAND_CAR_BACKWARD            3
#define COMMAND_CAR_TURNLEFT            4
#define COMMAND_CAR_TURNRIGHT           5
#define COMMAND_GET_CAR_SPEED           6

#define COMMAND_CONTROLLER_VIBRATE      100

typedef struct command_sender *command_sender_t;

struct command_sender
{
    char *name;
    void (*send)(rt_int8_t cmd, void *param, rt_uint16_t len);
};

struct command
{
    rt_int8_t   robot_cmd;
    rt_err_t    (*handler)(rt_int8_t cmd, void *param);
};

rt_err_t command_register(rt_int8_t cmd, rt_err_t (*handler)(rt_int8_t cmd, void *param));
rt_err_t command_unregister(rt_int8_t cmd);
rt_err_t command_handle(rt_int8_t cmd, void *param);
rt_err_t command_send(command_sender_t sender, rt_int8_t cmd, void *param, rt_uint16_t len);

#endif
