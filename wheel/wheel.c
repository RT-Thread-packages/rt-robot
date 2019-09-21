/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */

#include "wheel.h"

#define DBG_SECTION_NAME  "wheel"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

wheel_t wheel_create(motor_t w_motor, encoder_t w_encoder, controller_t w_controller, float radius, rt_uint16_t gear_ratio)
{
    //radius must not be zero, protect:
    if(radius == 0.0f) 
    {
        return RT_NULL;
    }

    // 1. Malloc memory for wheel
    wheel_t new_wheel = (wheel_t) rt_malloc(sizeof(struct wheel));
    if(new_wheel == RT_NULL)
    {
        LOG_E("Falied to allocate memory for new wheel");
        return RT_NULL;
    }

    // 2. Initialize wheel
    new_wheel -> w_motor      = w_motor;
    new_wheel -> w_encoder    = w_encoder;
    new_wheel -> w_controller = w_controller;
    new_wheel -> radius       = radius;
    new_wheel -> gear_ratio   = gear_ratio;

    // 3. pre compute the speed to rpm transform ;)
    new_wheel -> speed_to_rpm = 1.0 / (w->radius * 2.0 * 60.0 * PI);

    return new_wheel;
}

rt_err_t wheel_destroy(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    LOG_D("Free wheel");

    motor_destroy(whl->w_motor);
    encoder_destroy(whl->w_encoder);
    controller_destroy(whl->w_controller);

    rt_free(whl);

    return RT_EOK;
}

rt_err_t wheel_enable(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    LOG_D("Enabling wheel");

    // Enable PWM for motor
    motor_enable(whl->w_motor);

    // Enable Encoder's interrupt
    encoder_enable(whl->w_encoder);

    // Enable control
    controller_enable(whl->w_controller);

    return RT_EOK;
}

rt_err_t wheel_disable(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    LOG_D("Disabling wheel");

    // Disable PWM for motor
    motor_disable(whl->w_motor);

    // Disable Encoder's interrupt
    encoder_disable(whl->w_encoder);
    
    // Disable control
    controller_disable(whl->w_controller);

    return RT_EOK;
}

rt_err_t wheel_reset(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    // Reset Controller
    controller_reset(whl->w_controller);

    // Reset Motor
    motor_reset(whl->w_motor);
    
    // Reset Encoder
    encoder_reset(whl->w_encoder);

    return RT_EOK;
}

/** speed = rpm x 60 x 2 x PI x radius **/
/** so : rpm = speed x 1.0f / (radius x 60 x 2 x PT) */
/** then : rpm = speed * speed_to_rpm_transform --> precomputed */
rt_err_t wheel_set_speed(wheel_t whl, double speed)
{
    RT_ASSERT(whl != RT_NULL);

    return wheel_set_rpm(whl, (rt_int16_t) (speed * whl->speed_to_rpm));
}

rt_err_t wheel_set_rpm(wheel_t whl, rt_int16_t rpm)
{
    RT_ASSERT(whl != RT_NULL);

    controller_set_target(whl->w_controller, rpm);
    if(whl->w_controller->target == rpm)
    {
        return RT_EOK;
    }
    else
    {
        return RT_ERROR;
    }
}

void wheel_update(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    // Get current rpm
    whl->rpm = encoder_measure_rpm(whl->w_encoder);

    // Automatic control calculation
    controller_update(whl->w_controller, whl->rpm);

    // Set speed
    motor_run(whl->w_motor, whl->w_controller->output);
}

void wheel_stop(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    motor_stop(whl->w_motor);
}
