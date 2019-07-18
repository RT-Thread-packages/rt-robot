#include <rtthread.h>
#include <chassis.h>

typedef struct controller *controller_t;

// enum controller_type 
// {
//     PS2 = 0,
//     BLE,
//     TCP
// };

struct controller
{
    // enum    controller_type    type;

    rt_mailbox_t mb;
    rt_thread_t tid_controller;

    int     (*init)(void);
    int     (*enable)(void);
    int     (*disable)(void);

    int     (*contrl_cmd_handler)(rt_int8_t cmd);

};

controller_t controller_create(void);
void controller_destroy(controller_t contrl);

void controller_enable(void);
void controller_disable(void);
