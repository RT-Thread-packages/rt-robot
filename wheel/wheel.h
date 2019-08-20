#ifndef __WHEEL_H__
#define __WHEEL_H__

#include <rtthread.h>
#include <encoder.h>
#include <motor.h>
#include <controller.h>

#define PI 3.1415926

typedef struct wheel *wheel_t;

struct wheel
{
    motor_t         w_motor;
    encoder_t       w_encoder;
    controller_t    w_controller;

    rt_int16_t      rpm;
    float           radius;
    rt_uint16_t     gear_ratio;
};

wheel_t     wheel_create(motor_t w_motor, encoder_t w_encoder, controller_t w_controller, float radius, rt_uint16_t gear_ratio);
rt_err_t    wheel_destroy(wheel_t wheel);

rt_err_t    wheel_enable(wheel_t whl);
rt_err_t    wheel_disable(wheel_t whl);
rt_err_t    wheel_reset(wheel_t whl);

/** speed = rpm x 60 x 2 x PI x radius **/
rt_err_t    wheel_set_speed(wheel_t whl, double speed);
rt_err_t    wheel_set_rpm(wheel_t whl, rt_int16_t rpm);

void        wheel_update(wheel_t whl);
void        wheel_stop(wheel_t whl);

// Maximum Speed
// Wheel Thread

#endif
