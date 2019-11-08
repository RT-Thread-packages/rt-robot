/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 * 2019-08-26     sogwms       Add move api
 */
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

rt_err_t chassis_destroy(chassis_t chas)
{
    RT_ASSERT(chas != RT_NULL);

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Free wheel %d", i);
        wheel_destroy(chas->c_wheels[i]);
    }
    kinematics_destroy(chas->c_kinematics);

    rt_free(chas);

    return RT_EOK;
}

rt_err_t chassis_enable(chassis_t chas)
{
    RT_ASSERT(chas != RT_NULL);

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Enabling wheel %d", i);
        wheel_enable(chas->c_wheels[i]);
    }

    return RT_EOK;
}

rt_err_t chassis_disable(chassis_t chas)
{
    RT_ASSERT(chas != RT_NULL);

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Disabling wheel %d", i);
        wheel_disable(chas->c_wheels[i]);
    }

    return RT_EOK;
}

rt_err_t chassis_reset(chassis_t chas)
{
    RT_ASSERT(chas != RT_NULL);

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Reset wheel %d", i);
        wheel_reset(chas->c_wheels[i]);
    }
    kinematics_reset(chas->c_kinematics);

    return RT_EOK;
}

rt_err_t chassis_set_velocity(chassis_t chas, struct velocity target_velocity)
{
    RT_ASSERT(chas != RT_NULL);

    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    chassis_set_rpm(chas, res_rpm);

    return RT_EOK;
}

rt_err_t chassis_set_rpm(chassis_t chas, rt_int16_t target_rpm[])
{
    RT_ASSERT(chas != RT_NULL);

    // Set new speed
    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        LOG_I("Set wheel %d speed %d rpm", i, target_rpm[i]);
        wheel_set_rpm(chas->c_wheels[i], target_rpm[i]);
    }

    return RT_EOK;
}

rt_err_t chassis_update(chassis_t chas)
{
    RT_ASSERT(chas != RT_NULL);

    for(int i = 0; i < chas->c_kinematics->total_wheels; i++)
    {
        wheel_update(chas->c_wheels[i]);
    }
    return RT_EOK;
}

rt_err_t chassis_straight(chassis_t chas, float linear_x)
{
    RT_ASSERT(chas != RT_NULL);

    struct velocity target_velocity = {
        .linear_x = linear_x,
        .linear_y = 0.0f,
        .angular_z = 0.0f
    };
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

rt_err_t chassis_move(chassis_t chas, float linear_y)
{
    RT_ASSERT(chas != RT_NULL);

    struct velocity target_velocity = {
        .linear_x = 0.0f,
        .linear_y = linear_y,
        .angular_z = 0.0f
    };
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

rt_err_t chassis_rotate(chassis_t chas, float angular_z)
{
    RT_ASSERT(chas != RT_NULL);

    struct velocity target_velocity = {
        .linear_x = 0.0f,
        .linear_y = 0.0f,
        .angular_z = angular_z
    };
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

rt_err_t chassis_set_velocity_x(chassis_t chas, float linear_x)
{
    RT_ASSERT(chas != RT_NULL);

    chas->c_velocity.linear_x = linear_x;
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, chas->c_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

rt_err_t chassis_set_velocity_y(chassis_t chas, float linear_y)
{
    RT_ASSERT(chas != RT_NULL);

    chas->c_velocity.linear_y = linear_y;
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, chas->c_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

rt_err_t chassis_set_velocity_z(chassis_t chas, float angular_z)
{
    RT_ASSERT(chas != RT_NULL);
    
    chas->c_velocity.angular_z = angular_z;
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, chas->c_velocity);
    return chassis_set_rpm(chas, res_rpm);
}
