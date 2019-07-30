#include "wheel.h"

#define DBG_SECTION_NAME  "wheel"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

wheel_t wheel_create(motor_t w_motor, encoder_t w_encoder, auto_control_t w_control, float radius, rt_uint16_t gear_ratio)
{
    // 1. Malloc memory for wheel
    wheel_t new_wheel = (wheel_t) rt_malloc(sizeof(struct wheel));
    if(new_wheel == RT_NULL)
    {
        LOG_E("Falied to allocate memory for dc wheel");
        return RT_NULL;
    }

    // 2. Initialize wheel
    new_wheel -> w_motor    = w_motor;
    new_wheel -> w_encoder  = w_encoder;
    new_wheel -> w_control  = w_control;
    new_wheel -> radius     = radius;
    new_wheel -> gear_ratio = gear_ratio;

    return new_wheel;
}

void wheel_destroy(wheel_t wheel)
{
    LOG_D("Free wheel");

    motor_destroy(wheel->w_motor);
    encoder_destroy(wheel->w_encoder);
    auto_control_destroy(wheel->w_control);

    rt_free(wheel);
}

rt_err_t wheel_enable(wheel_t whl)
{
    // TODO
    LOG_D("Enabling wheel");

    // Enable PWM for motor
    motor_enable(whl->w_motor);

    // Enable Encoder's interrupt
    encoder_enable(whl->w_encoder);
    rt_thread_mdelay(whl->w_encoder->sample_time);

    // Enable control
    auto_control_enable(whl->w_control);

    return RT_EOK;
}

rt_err_t wheel_disable(wheel_t whl)
{
    // TODO
    LOG_D("Disabling wheel");

    // Disable PWM for motor
    motor_disable(whl->w_motor);

    // Disable Encoder's interrupt
    encoder_disable(whl->w_encoder);
    
    // Disable control
    auto_control_disable(whl->w_control);

    return RT_EOK;
}

rt_err_t wheel_reset(wheel_t whl)
{
    // TODO

    return RT_EOK;
}

/** speed = rpm x 60 x 2 x PI x radius **/
rt_err_t wheel_set_speed(wheel_t whl, double speed)
{
    return wheel_set_rpm(whl, (rt_int16_t) (speed / 60.0 / 2.0 / whl->radius / PI));
}

rt_err_t wheel_set_rpm(wheel_t whl, rt_int16_t rpm)
{
    LOG_D("Set wheel speed %d rpm", rpm);
    auto_control_set_target(whl->w_control, rpm);
    if(whl->w_control->target == rpm)
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
    // TODO
    // Get current rpm
    whl->rpm = encoder_measure_rpm(whl->w_encoder);

    // Automatic control calculation
    auto_control_update(whl->w_control, whl->rpm);

    // Set speed
    motor_run(whl->w_motor, whl->w_control->output);
}

void wheel_stop(wheel_t whl)
{
    motor_stop(whl->w_motor);
}

// Maximum Speed
// Wheel Thread
