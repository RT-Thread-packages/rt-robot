#include "motor.h"

#define DBG_SECTION_NAME  "motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

motor_t motor_create(int (*init)(void), int (*enable)(void), int (*disable)(void), int (*set_speed)(rt_int8_t percentage), enum motor_type type)
{
    // 1. Malloc memory
    motor_t new_motor = (motor_t)rt_malloc(sizeof(struct motor));
    if(new_motor == RT_NULL)
    {
        LOG_E("Falied to allocate memory for dc motor");
        return RT_NULL;
    }

    new_motor->type      = type;
    new_motor->init      = init;
    new_motor->enable    = enable;
    new_motor->disable   = disable;
    new_motor->set_speed = set_speed;

    new_motor->init();

    return new_motor;
}

rt_err_t motor_enable(motor_t mot)
{
    // Enable PWM
    LOG_D("Enabling motor");
    mot->enable();

    return RT_EOK;
}

rt_err_t motor_disable(motor_t mot)
{
    // Disable PWM
    LOG_D("Disabling motor");
    mot->disable();

    return RT_EOK;
}

void motor_run(motor_t mot, rt_int8_t pwm)
{
    // Set speed (pwm) to desired value
    // LOG_D("Set motor speed %d pwm", pwm);
    mot->set_speed(pwm);
}

void motor_stop(motor_t mot)
{
    // Set Speed to 0
    motor_run(mot, 0);
}

void motor_destroy(motor_t mot)
{
    // Disable first
    motor_disable(mot);

    // Free memory
    LOG_D("Free motor");

    rt_free(mot);
}
