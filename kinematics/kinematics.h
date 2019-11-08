/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */

#ifndef __KINEMATICS_H__
#define __KINEMATICS_H__

#include <rtthread.h>

enum base {
    TWO_WD = 0, 
    FOUR_WD, 
    ACKERMANN, 
    MECANUM
};

// rad/min
struct rpm
{
    int motor1;
    int motor2;
    int motor3;
    int motor4;
};

// m/s
struct velocity
{
    float linear_x;
    float linear_y;
    float angular_z;
};

typedef struct kinematics *kinematics_t;

struct kinematics
{
    enum base   k_base;
    float       length_x;
    float       length_y;
    float       wheel_radius;
    int         total_wheels;
};

kinematics_t    kinematics_create(enum base k_base, float length_x, float length_y, float wheel_radius);
void            kinematics_destroy(kinematics_t kinematics);
rt_err_t        kinematics_reset(kinematics_t kin);

void            kinematics_get_rpm(struct kinematics kin, struct velocity target_vel, rt_int16_t *rpm);
void            kinematics_get_velocity(struct kinematics kin, struct rpm current_rpm, struct velocity *velocity);

#endif
