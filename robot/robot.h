#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <rtthread.h>
#include <chassis.h>

typedef struct robot *robot_t;

enum robot_type 
{
    MOBILE_ROBOT,
    ARM_ROBOT,
    AERIAL_ROBOT
};

struct robot
{
    enum    robot_type    type;

    int     (*init)(void);
    int     (*enable)(void);
    int     (*disable)(void);
    int     (*execute)(rt_int8_t cmd, void *args);
};

#endif
