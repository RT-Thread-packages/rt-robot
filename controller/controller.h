#include <rtthread.h>

// command
#define CONTROLLER_CMD_REMIND    1

// digital bit
#define CONTROLLER_BITMASK_START        (1 << 0)
#define CONTROLLER_BITMASK_STOP         (1 << 1)

typedef struct controller *controller_t;

enum controller_type 
{
    PS2,
};

struct controller_analog
{
    int8_t x;       // -100 ~ 100 <> percentage
    int8_t y;
    int8_t z;
};

struct controller
{
    enum controller_type type;
    struct controller_analog analog;
    uint32_t digital;
};

controller_t    controller_create(enum controller_type type);
void            controller_destroy(controller_t controller);
rt_err_t        controller_enable(controller_t controller);
rt_err_t        controller_disable(controller_t controller);
rt_err_t        controller_get(controller_t controller);
rt_err_t        controller_set(controller_t controller, int cmd, void *arg);

