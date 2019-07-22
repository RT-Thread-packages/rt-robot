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

    // 机器人对外传递信息
    rt_mailbox_t mb;

    int     (*init)(void);
    int     (*enable)(void);
    int     (*disable)(void);
    int     (*robot_exec_cmd)(rt_int8_t cmd, void *args);
};
