#include "chassis.h"

#define DBG_SECTION_NAME  "chassis"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

chassis_t chassis_create(wheel_t* c_wheels, kinematics_t c_kinematics)
{
    // Malloc memory for new chassis
    chassis_t new_chassis = (chassis_t) rt_malloc(sizeof(struct chassis));
    if(new_chassis == RT_NULL)
    {
        LOG_E("Falied to allocate memory for chassis\n");
        return RT_NULL;
    }

    new_chassis -> c_wheels      = c_wheels;
    new_chassis -> c_kinematics  = c_kinematics;

    return new_chassis;
}

void chassis_destroy(chassis_t chas)
{
    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Free wheel %d", i);
        wheel_destroy(chas->c_wheels[i]);
    }
    kinematics_destroy(chas->c_kinematics);

    rt_free(chas);
}

rt_err_t chassis_enable(chassis_t chas)
{
    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Enabling wheel %d", i);
        wheel_enable(chas->c_wheels[i]);
    }

    return RT_EOK;
}

rt_err_t chassis_disable(chassis_t chas)
{
    // TODO

    return RT_EOK;
}

rt_err_t chassis_reset(chassis_t chas)
{
    // TODO

    return RT_EOK;
}

rt_err_t chassis_set_velocity(chassis_t chas, struct velocity target_velocity)
{
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    chassis_set_rpm(chas, res_rpm);

    return RT_EOK;
}

rt_err_t chassis_set_rpm(chassis_t chas, rt_int16_t target_rpm[])
{
    // Set new speed
    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Set wheel %d speed %d rpm", i, target_rpm[i]);
        wheel_set_rpm(chas->c_wheels[i], target_rpm[i]);
    }

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        wheel_update(chas->c_wheels[i]);
    }

    return RT_EOK;
}

rt_err_t chassis_update(chassis_t chas)
{
    // TODO
    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        wheel_update(chas->c_wheels[i]);
    }
    return RT_EOK;
}
