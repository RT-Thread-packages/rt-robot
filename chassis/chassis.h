#ifndef __CHASSIS_H__
#define __CHASSIS_H__

#include <kinematics.h>
#include <wheel.h>

typedef struct chassis *chassis_t;

struct chassis
{
    wheel_t*        c_wheels;
    kinematics_t    c_kinematics;
    struct velocity c_velocity;
};

chassis_t   chassis_create(wheel_t* c_wheel, kinematics_t c_kinematics);
void        chassis_destroy(chassis_t chas);

rt_err_t    chassis_enable(chassis_t chas);
rt_err_t    chassis_disable(chassis_t chas);
rt_err_t    chassis_reset(chassis_t chas);

rt_err_t    chassis_set_velocity(chassis_t chas, struct velocity target_velocity);
rt_err_t    chassis_set_rpm(chassis_t chas, rt_int16_t target_rpm[]);

rt_err_t    chassis_update(chassis_t chas);

#endif
