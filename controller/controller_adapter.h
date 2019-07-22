#include <rtthread.h>
#include <robot.h>
#include <controller.h>

typedef struct controller_adapter *controller_adapter_t;

// enum controller_type 
// {
//     PS2,
// };

struct controller_adapter_table
{
    rt_int8_t ctrl_cmd;
    rt_int8_t rob_cmd;
};

struct controller_adapter
{
    // enum controller_type    c_type;
    // enum robot_type         r_type;
    rt_int8_t               table_size;
    rt_int8_t               table_max;
    struct controller_adapter_table *table;

    rt_mailbox_t contrl_mb;
    rt_mailbox_t robot_mb;

    int     (*contrl_cmd_handler)(rt_int8_t cmd, void *args);
    int     (*robot_cmd_handler)(rt_int8_t cmd, void *args);
};

controller_adapter_t    controller_adapter_create(rt_mailbox_t robot_mb, rt_mailbox_t contrl_mb);
void                    controller_adapter_destroy(controller_adapter_t adpt);

rt_err_t        controller_adapter_enable(controller_adapter_t adpt);
rt_err_t        controller_adapter_disable(controller_adapter_t adpt);


rt_err_t        controller_adapter_bind_cmd(controller_adapter_t adpt, rt_int8_t contrl_cmd, rt_int8_t rob_cmd);
static rt_int8_t controller_adapter_convert_cmd(controller_adapter_t adapt, rt_int8_t contrl_cmd, void *args);
rt_err_t        controller_adapter_send_cmd(controller_adapter_t adpt, rt_int8_t contrl_cmd, void *args);
