#include "motor.h"

#define DBG_SECTION_NAME  "motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

motor_t motor_create(rt_size_t size)
{
    motor_t new_motor = (motor_t)rt_malloc(size);
    if (new_motor == RT_NULL)
    {
        LOG_E("Falied to allocate memory for motor");
        return RT_NULL;
    }

    return new_motor;
}

rt_err_t motor_enable(motor_t mot)
{
    RT_ASSERT(mot != RT_NULL);

    // Enable PWM
    LOG_D("Enabling motor");
    mot->enable(mot);

    return RT_EOK;
}

rt_err_t motor_disable(motor_t mot)
{
    RT_ASSERT(mot != RT_NULL);

    // Disable PWM
    LOG_D("Disabling motor");
    mot->disable(mot);

    return RT_EOK;
}

rt_err_t motor_run(motor_t mot, rt_int16_t thousands)
{
    RT_ASSERT(mot != RT_NULL);

    // Set speed (pwm) to desired value
    // LOG_D("Set motor speed %d pwm", pwm);
    mot->set_speed(mot, thousands);

    return RT_EOK;
}

rt_err_t motor_stop(motor_t mot)
{
    RT_ASSERT(mot != RT_NULL);

    // Set Speed to 0
    motor_run(mot, 0);

    return RT_EOK;
}

rt_err_t motor_destroy(motor_t mot)
{
    RT_ASSERT(mot != RT_NULL);
    
    // Disable first
    motor_disable(mot);

    // Free memory
    LOG_D("Free motor");

    rt_free(mot);

    return RT_EOK;
}
