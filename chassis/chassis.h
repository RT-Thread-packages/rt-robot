/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */
#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include <kinematics.h>
#include <wheel.h>

#define CHASSIS_VELOCITY_LINEAR_MAXIMUM         0.1f                                                  // m/s
#define CHASSIS_VELOCITY_ANGULAR_MAXIMUM        5.0f * CHASSIS_VELOCITY_LINEAR_MAXIMUM                // rad/s

typedef struct chassis *chassis_t;

struct chassis
{
    wheel_t*        c_wheels;
    kinematics_t    c_kinematics;
    struct velocity c_velocity;
};

chassis_t   chassis_create(wheel_t* c_wheel, kinematics_t c_kinematics);
rt_err_t    chassis_destroy(chassis_t chas);

rt_err_t    chassis_enable(chassis_t chas);
rt_err_t    chassis_disable(chassis_t chas);
rt_err_t    chassis_reset(chassis_t chas);

rt_err_t    chassis_set_velocity(chassis_t chas, struct velocity target_velocity);
rt_err_t    chassis_set_rpm(chassis_t chas, rt_int16_t target_rpm[]);

rt_err_t    chassis_update(chassis_t chas);

rt_err_t    chassis_straight(chassis_t chas, float linear_x);
rt_err_t    chassis_move(chassis_t chas, float linear_y);
rt_err_t    chassis_rotate(chassis_t chas, float angular_z);

rt_err_t    chassis_set_velocity_x(chassis_t chas, float linear_x);
rt_err_t    chassis_set_velocity_y(chassis_t chas, float linear_y);
rt_err_t    chassis_set_velocity_z(chassis_t chas, float angular_z);

#endif
