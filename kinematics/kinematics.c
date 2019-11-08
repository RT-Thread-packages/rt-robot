/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */

#include "kinematics.h"

#define DBG_SECTION_NAME  "kinematics"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

kinematics_t kinematics_create(enum base k_base, float length_x, float length_y, float wheel_radius)
{
    kinematics_t new_kinematics = (kinematics_t)rt_malloc(sizeof(struct kinematics));
    if(new_kinematics == RT_NULL)
    {
        LOG_E("Failed to malloc memory for kinematics\n");
        return RT_NULL;
    }

    new_kinematics->k_base       = k_base;
    new_kinematics->length_x     = length_x;
    new_kinematics->length_y     = length_y;
    new_kinematics->wheel_radius = wheel_radius;

    if(k_base == TWO_WD)
    {
        new_kinematics->total_wheels = 2;
    }
    if(k_base == FOUR_WD)
    {
        new_kinematics->total_wheels = 4;
    }
    if(k_base == ACKERMANN)
    {
        new_kinematics->total_wheels = 3;
    }
    if(k_base == MECANUM)
    {
        new_kinematics->total_wheels = 4;
    }

    return new_kinematics;
}

void kinematics_destroy(kinematics_t kin)
{
    RT_ASSERT(kin != RT_NULL);
    LOG_I("Free Kinematics");
    rt_free(kin);
}

rt_err_t kinematics_reset(kinematics_t kin)
{
    // TODO
    RT_ASSERT(kin != RT_NULL);
    LOG_I("Reset kinematics");
    return RT_EOK;
}

/* Return desired rpm for each motor given target velocity */
void kinematics_get_rpm(struct kinematics kin, struct velocity target_vel, rt_int16_t *rpm)
{
    // TODO
    struct rpm cal_rpm;
    rt_int16_t res_rpm[4] = {0};

    float linear_vel_x_mins;
    float linear_vel_y_mins;
    float angular_vel_z_mins;
    float tangential_vel;

    float x_rpm;
    float y_rpm;
    float tan_rpm;

    if(kin.k_base == TWO_WD || kin.k_base == FOUR_WD ||  kin.k_base == ACKERMANN)
    {
        target_vel.linear_y = 0;
    }

    //convert m/s to m/min
    linear_vel_x_mins = target_vel.linear_x * 60;
    linear_vel_y_mins = target_vel.linear_y * 60;

    //convert rad/s to rad/min
    angular_vel_z_mins = target_vel.angular_z * 60;

    tangential_vel = angular_vel_z_mins * ((kin.length_x / 2) + (kin.length_y / 2));

    x_rpm   = linear_vel_x_mins / kin.wheel_radius;
    y_rpm   = linear_vel_y_mins / kin.wheel_radius;
    tan_rpm = tangential_vel / kin.wheel_radius;

    // front-left motor
    cal_rpm.motor1 = x_rpm - y_rpm - tan_rpm;

    // front-right motor
    cal_rpm.motor2 = x_rpm + y_rpm + tan_rpm;

    // rear-left motor
    cal_rpm.motor3 = x_rpm + y_rpm - tan_rpm;

    // rear-right motor
    cal_rpm.motor4 = x_rpm - y_rpm + tan_rpm;

    if(kin.k_base == TWO_WD)
    {
        res_rpm[0] = cal_rpm.motor3;
        res_rpm[1] = cal_rpm.motor4;
    }
    else if(kin.k_base == FOUR_WD)
    {
        res_rpm[0] = cal_rpm.motor1;
        res_rpm[1] = cal_rpm.motor2;
        res_rpm[2] = cal_rpm.motor3;
        res_rpm[3] = cal_rpm.motor4;
    }
    else if(kin.k_base == ACKERMANN)
    {
        res_rpm[0] = target_vel.angular_z;
        res_rpm[1] = cal_rpm.motor3;
        res_rpm[2] = cal_rpm.motor4;
    }
    else if(kin.k_base == MECANUM)
    {
        res_rpm[0] = cal_rpm.motor1;
        res_rpm[1] = cal_rpm.motor2;
        res_rpm[2] = cal_rpm.motor3;
        res_rpm[3] = cal_rpm.motor4;
    }
    else
    {
        return;
    }
    
    for (int i = 0; i < 4; i++)
    {
        rpm[i] = res_rpm[i];
    }
}

/* Return current velocity given rpm of each motor */
void kinematics_get_velocity(struct kinematics kin, struct rpm current_rpm, struct velocity *velocity)
{
    // TODO
    struct velocity res_vel;

    int total_wheels = 0;
    if(kin.k_base == TWO_WD) total_wheels = 2;
    if(kin.k_base == FOUR_WD) total_wheels = 4;
    if(kin.k_base == ACKERMANN) total_wheels = 2;
    if(kin.k_base == MECANUM) total_wheels = 4;

    float average_rps_x;
    float average_rps_y;
    float average_rps_a;

    //convert average revolutions per minute to revolutions per second
    average_rps_x = ((float)(current_rpm.motor1 + current_rpm.motor2 + current_rpm.motor3 + current_rpm.motor4) / total_wheels) / 60; // RPM
    res_vel.linear_x = average_rps_x * kin.wheel_radius; // m/s

    //convert average revolutions per minute in y axis to revolutions per second
    average_rps_y = ((float)(-current_rpm.motor1 + current_rpm.motor2 + current_rpm.motor3 - current_rpm.motor4) / total_wheels) / 60; // RPM
    if(kin.k_base == MECANUM)
        res_vel.linear_y = average_rps_y * kin.wheel_radius; // m/s
    else
        res_vel.linear_y = 0;

    //convert average revolutions per minute to revolutions per second
    average_rps_a = ((float)(-current_rpm.motor1 + current_rpm.motor2 - current_rpm.motor3 + current_rpm.motor4) / total_wheels) / 60;
    res_vel.angular_z =  (average_rps_a * kin.wheel_radius) / ((kin.length_x / 2) + (kin.length_y / 2)); //  rad/s

    rt_memcpy(velocity, &res_vel, sizeof(struct velocity));
}
