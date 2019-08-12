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

// #define COMMAND_CAR_FORWARD_WITH_PARAM

#define COMMAND_RC_VIBRATE              -1

struct command_info
{
    void       *target;
    rt_int16_t cmd;
    void       *param;
};

typedef struct command_info *command_info_t;

rt_err_t command_register(rt_int16_t cmd, void (*handler)(command_info_t info));
rt_err_t command_unregister(rt_int16_t cmd);
rt_err_t command_send(command_info_t info);

#endif
