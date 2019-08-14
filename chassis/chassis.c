#include "chassis.h"
#include <command.h>

#define DBG_SECTION_NAME  "chassis"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define DEFAULT_VELOCITY_LINEAR_X       0.2
#define DEFAULT_VELOCITY_LINEAR_Y       0.2
#define DEFAULT_VELOCITY_ANGULAR_Z      1

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

rt_err_t chassis_straight(chassis_t chas, float linear_x)
{
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
    struct velocity target_velocity = {
        .linear_x = 0.0f,
        .linear_y = 0.0f,
        .angular_z = angular_z
    };
    rt_int16_t* res_rpm = kinematics_get_rpm(*chas->c_kinematics, target_velocity);
    return chassis_set_rpm(chas, res_rpm);
}

static void chassis_stop(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_straight(info->target, 0);
    }
}

static void chassis_forward(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_straight(info->target, DEFAULT_VELOCITY_LINEAR_X);
    }
}

static void chassis_backward(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_straight(info->target, -DEFAULT_VELOCITY_LINEAR_X);
    }
}

static void chassis_rotate_left(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_rotate(info->target, DEFAULT_VELOCITY_ANGULAR_Z);
    }
}

static void chassis_rotate_right(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_rotate(info->target, -DEFAULT_VELOCITY_ANGULAR_Z);
    }
}

static void chassis_move_left(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_move(info->target, DEFAULT_VELOCITY_LINEAR_Y);
    }
}

static void chassis_move_right(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        chassis_move(info->target, DEFAULT_VELOCITY_LINEAR_Y);
    }
}

static void chassis_forward_with_param(command_info_t info, void *param, rt_uint16_t size)
{
    if (info->target != RT_NULL)
    {
        if (size == sizeof(struct cmd_dt_velocity))
        {
            struct cmd_dt_velocity *velocity_info = (struct cmd_dt_velocity *)param;
            chassis_straight(info->target, velocity_info->data.linear_x);
        }
    }
}

static int chassis_command_register(void)
{
    // TODO
    command_register(COMMAND_CAR_STOP, chassis_stop);
    
    command_register(COMMAND_CAR_FORWARD_WITH_PARAM, chassis_forward_with_param);

    return RT_EOK;
}

INIT_APP_EXPORT(chassis_command_register);
