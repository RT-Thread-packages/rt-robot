#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <rtthread.h>
#include <rtdevice.h>

typedef struct motor *motor_t;

enum motor_type
{
    SERVO = 0,
    DC_MOTOR,
    // STEPPER_MOTOR,
    // CAN_MOTOR,
};

struct motor
{
    enum    motor_type                  type;
	int 	index;
    int     (*init)(int index);
    int     (*enable)(int index);
    int     (*disable)(int index);
    int     (*set_speed)(int index,rt_int8_t percentage);
};

motor_t   motor_create(int motor_index,
					   int (*init)(int index),
                       int (*enable)(int index), 
                       int (*disable)(int index),
                       int (*set_speed)(int index,rt_int8_t percentage),
                       enum motor_type type);
void      motor_destroy(motor_t mot);

rt_err_t  motor_enable(motor_t mot);
rt_err_t  motor_disable(motor_t mot);

void      motor_run(motor_t motor, rt_int8_t pwm);
void      motor_stop(motor_t motor);

#endif
