#include <rtthread.h>
#include <rtdevice.h>

#define DBG_SECTION_NAME  "dc_motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define PWM_PERIOD 50000

// MOTOR
#define LEFT_FORWARD_PWM            "pwm4"
#define LEFT_FORWARD_PWM_CHANNEL    3
#define LEFT_BACKWARD_PWM           "pwm4"
#define LEFT_BACKWARD_PWM_CHANNEL   4

#define RIGHT_FORWARD_PWM           "pwm2"
#define RIGHT_FORWARD_PWM_CHANNEL   3
#define RIGHT_BACKWARD_PWM          "pwm2"
#define RIGHT_BACKWARD_PWM_CHANNEL  4

static struct rt_device_pwm *left_forward_pwmdev;
static struct rt_device_pwm *left_backward_pwmdev;
static struct rt_device_pwm *right_forward_pwmdev;
static struct rt_device_pwm *right_backward_pwmdev;

int left_motor_init(void)
{
    left_forward_pwmdev   = (struct rt_device_pwm *)rt_device_find(LEFT_FORWARD_PWM);
    left_backward_pwmdev  = (struct rt_device_pwm *)rt_device_find(LEFT_BACKWARD_PWM);

    if (left_forward_pwmdev == RT_NULL)
    {
        LOG_E("Can't find PWM device %s for dc forward motor!", LEFT_FORWARD_PWM);
        return RT_NULL;
    }
    if (left_backward_pwmdev == RT_NULL)
    {
        LOG_E("Can't find PWM device %s for dc forward motor!", LEFT_BACKWARD_PWM);
        return RT_NULL;
    }

	rt_pwm_set(left_forward_pwmdev, LEFT_FORWARD_PWM_CHANNEL, PWM_PERIOD, 0);
    rt_pwm_set(left_backward_pwmdev, LEFT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, 0);
    
	return RT_EOK;
}

int left_motor_enable(void)
{
    // Enable PWM
    LOG_D("Enabling motor");

    rt_pwm_enable(left_backward_pwmdev, LEFT_BACKWARD_PWM_CHANNEL);
    rt_pwm_enable(left_forward_pwmdev, LEFT_FORWARD_PWM_CHANNEL);
    
    return 0;
}

int left_motor_disable(void)
{
     // Disable PWM
    LOG_D("Disabling motor");

    rt_pwm_disable(left_backward_pwmdev, LEFT_BACKWARD_PWM_CHANNEL);
    rt_pwm_disable(left_forward_pwmdev, LEFT_FORWARD_PWM_CHANNEL);
    
    return 0;
}

int left_motor_set_speed(rt_int8_t percentage)
{
    // Set speed (pwm) to desired value
    // LOG_D("Set motor speed %d pwm", percentage);

    if(percentage > 0)
    {
        rt_pwm_set(left_backward_pwmdev, LEFT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, 0);
        rt_pwm_set(left_forward_pwmdev, LEFT_FORWARD_PWM_CHANNEL, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
    }
    else
    {
        rt_pwm_set(left_forward_pwmdev, LEFT_FORWARD_PWM_CHANNEL, PWM_PERIOD, 0);
        rt_pwm_set(left_backward_pwmdev, LEFT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
    }
    
    return 0;
}

int right_motor_init(void)
{
	right_forward_pwmdev  = (struct rt_device_pwm *)rt_device_find(RIGHT_FORWARD_PWM);
    right_backward_pwmdev = (struct rt_device_pwm *)rt_device_find(RIGHT_BACKWARD_PWM);
	
    if (right_forward_pwmdev == RT_NULL)
    {
        LOG_E("Can't find PWM device %s for dc forward motor!", RIGHT_FORWARD_PWM);
        return RT_NULL;
    }
    if (right_backward_pwmdev == RT_NULL)
    {
        LOG_E("Can't find PWM device %s for dc forward motor!", RIGHT_BACKWARD_PWM);
        return RT_NULL;
    }

    rt_pwm_set(right_backward_pwmdev, RIGHT_FORWARD_PWM_CHANNEL, PWM_PERIOD, 0);
    rt_pwm_set(right_backward_pwmdev, RIGHT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, 0);
    
    return RT_EOK;
}

int right_motor_enable(void)
{
    // Enable PWM
    LOG_D("Enabling motor");

    rt_pwm_enable(right_backward_pwmdev, RIGHT_BACKWARD_PWM_CHANNEL);
    rt_pwm_enable(right_forward_pwmdev, RIGHT_FORWARD_PWM_CHANNEL);
    
    return 0;
}

int right_motor_disable(void)
{
     // Disable PWM
    LOG_D("Disabling motor");

    rt_pwm_disable(right_backward_pwmdev, RIGHT_BACKWARD_PWM_CHANNEL);
    rt_pwm_disable(right_forward_pwmdev, RIGHT_FORWARD_PWM_CHANNEL);

    return 0;
}

int right_motor_set_speed(rt_int8_t percentage)
{
    // Set speed (pwm) to desired value
    // LOG_D("Set motor speed %d pwm", percentage);

    if(percentage > 0)
    {
        rt_pwm_set(right_backward_pwmdev, RIGHT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, 0);
        rt_pwm_set(right_forward_pwmdev, RIGHT_FORWARD_PWM_CHANNEL, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
    }
    else
    {
        rt_pwm_set(right_forward_pwmdev, RIGHT_FORWARD_PWM_CHANNEL, PWM_PERIOD, 0);
        rt_pwm_set(right_backward_pwmdev, RIGHT_BACKWARD_PWM_CHANNEL, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
    }
    
    return 0;
}
