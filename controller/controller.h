#include <rtthread.h>
#include <chassis.h>

typedef struct controller *controller_t;

enum controller_type 
{
    PS2,
};

struct controller_table
{
    rt_int8_t remote;
    rt_int8_t chassis;
};

struct controller
{
    enum controller_type    type;
    chassis_t               chassis;
    rt_int8_t               table_size;
    rt_int8_t               table_max;
    struct controller_table *table;
};

controller_t    controller_create(chassis_t chas, enum controller_type type);
void            controller_destroy(controller_t controller);
rt_err_t        controller_enable(controller_t controller);
rt_err_t        controller_disable(controller_t controller);
rt_err_t        controller_bind_command(controller_t controller, rt_int8_t cmd, rt_int8_t chas_cmd);
rt_err_t        controller_parse_command(controller_t controller, rt_int8_t cmd, void *args);
