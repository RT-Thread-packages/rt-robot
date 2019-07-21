
#include "dc_motor.h"

#define DBG_SECTION_NAME  "dc_motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#ifdef	ONE_PWM_DUAL_PIN
	struct dc_motor_cfg{
		char* pwm_name;
		rt_uint8_t channel;
		int pina_index;
		int pinb_index;
		struct rt_device_pwm * pwmdev;
	};
#endif	/* ONE_PWM_DUAL_PIN*/
	
#ifdef	DUAL_PWM
	struct dc_motor_cfg{
		char* pwm_a;
		rt_uint8_t channel_a;
		char* pwm_b;
		rt_uint8_t channel_b;
		struct rt_device_pwm * pwm_a_dev;
		struct rt_device_pwm * pwm_b_dev;
	};
#endif	/* DUAL_PWM*/
static struct dc_motor_cfg dc_motor_cfg_obj[] =
{
	#if 	DC_MOTOR_NUMBER > 0
		DC_MOTOR_PWM_CFG1,
	#endif
	#if 	DC_MOTOR_NUMBER > 1
		DC_MOTOR_PWM_CFG2,
	#endif
	#if 	DC_MOTOR_NUMBER > 2
		DC_MOTOR_PWM_CFG3,
	#endif
	#if 	DC_MOTOR_NUMBER > 3
		DC_MOTOR_PWM_CFG4,
	#endif
};


int dc_motor_creat(int motor_index)
{
#ifdef ONE_PWM_DUAL_PIN
	dc_motor_cfg_obj[motor_index].pwmdev   = (struct rt_device_pwm *)rt_device_find(dc_motor_cfg_obj[motor_index].pwm_name);
	if (dc_motor_cfg_obj[motor_index].pwmdev == RT_NULL)
	{
		LOG_E("Can't find PWM device %s for dc forward motor!", dc_motor_cfg_obj[motor_index].pwmdev);
		return RT_NULL;
	}
	rt_pwm_set(dc_motor_cfg_obj[motor_index].pwmdev, dc_motor_cfg_obj[motor_index].channel, PWM_PERIOD, 0);
	rt_pin_mode(dc_motor_cfg_obj[motor_index].pina_index,PIN_MODE_OUTPUT);
	rt_pin_mode(dc_motor_cfg_obj[motor_index].pinb_index,PIN_MODE_OUTPUT);
	
#endif
#ifdef DUAL_PWM
	dc_motor_cfg_obj[motor_index].pwm_a_dev   = (struct rt_device_pwm *)rt_device_find(dc_motor_cfg_obj[motor_index].pwm_a);
	dc_motor_cfg_obj[motor_index].pwm_b_dev   = (struct rt_device_pwm *)rt_device_find(dc_motor_cfg_obj[motor_index].pwm_b);
	if (dc_motor_cfg_obj[motor_index].pwm_a_dev == RT_NULL)
	{
		LOG_E("Can't find PWM device %s for dc forward motor!", dc_motor_cfg_obj[motor_index].pwm_a_dev);
		return RT_NULL;
	}
	if (dc_motor_cfg_obj[motor_index].pwm_b_dev == RT_NULL)
	{
		LOG_E("Can't find PWM device %s for dc forward motor!", dc_motor_cfg_obj[motor_index].pwm_b_dev);
		return RT_NULL;
	}
	rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_a_dev, dc_motor_cfg_obj[motor_index].channel_a, PWM_PERIOD, 0);
	rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_b_dev, dc_motor_cfg_obj[motor_index].channel_b, PWM_PERIOD, 0);
#endif
	
	return RT_EOK;
}

int dc_motor_enable(int motor_index)
{
    // Enable PWM
    LOG_D("Enabling motor");
#ifdef ONE_PWM_DUAL_PIN
	rt_pwm_enable(dc_motor_cfg_obj[motor_index].pwmdev, dc_motor_cfg_obj[motor_index].channel);
#endif
#ifdef DUAL_PWM
    rt_pwm_enable(dc_motor_cfg_obj[motor_index].pwm_a_dev, dc_motor_cfg_obj[motor_index].channel_a);
    rt_pwm_enable(dc_motor_cfg_obj[motor_index].pwm_b_dev, dc_motor_cfg_obj[motor_index].channel_b);
#endif
    return RT_EOK;
}

int dc_motor_disable(int motor_index)
{
	// Disable PWM
    LOG_D("Disabling motor");
#ifdef ONE_PWM_DUAL_PIN
	rt_pwm_disable(dc_motor_cfg_obj[motor_index].pwmdev, dc_motor_cfg_obj[motor_index].channel);	
#endif
#ifdef DUAL_PWM
    rt_pwm_disable(dc_motor_cfg_obj[motor_index].pwm_a_dev, dc_motor_cfg_obj[motor_index].channel_a);
    rt_pwm_disable(dc_motor_cfg_obj[motor_index].pwm_b_dev, dc_motor_cfg_obj[motor_index].channel_b);
#endif
    return RT_EOK;
}

int dc_motor_set_speed(int motor_index,rt_int8_t percentage)
{
    // Set speed (pwm) to desired value
    // LOG_D("Set motor speed %d pwm", percentage);

    if(percentage > 0)
    {
	#ifdef ONE_PWM_DUAL_PIN
		rt_pin_write(dc_motor_cfg_obj[motor_index].pina_index, PIN_HIGH);
		rt_pin_write(dc_motor_cfg_obj[motor_index].pinb_index, PIN_LOW);
		rt_pwm_set(dc_motor_cfg_obj[motor_index].pwmdev, dc_motor_cfg_obj[motor_index].channel, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
	#endif
	#ifdef DUAL_PWM
		rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_a_dev, dc_motor_cfg_obj[motor_index].channel_a, PWM_PERIOD, 0);
        rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_b_dev, dc_motor_cfg_obj[motor_index].channel_b, PWM_PERIOD, PWM_PERIOD / 100 * percentage);
	#endif
    }
    else
    {
	#ifdef ONE_PWM_DUAL_PIN
		rt_pin_write(dc_motor_cfg_obj[motor_index].pina_index, PIN_LOW);
		rt_pin_write(dc_motor_cfg_obj[motor_index].pinb_index, PIN_HIGH);
		rt_pwm_set(dc_motor_cfg_obj[motor_index].pwmdev, dc_motor_cfg_obj[motor_index].channel, PWM_PERIOD, -PWM_PERIOD / 100 * percentage);
	#endif
	#ifdef DUAL_PWM
		rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_a_dev, dc_motor_cfg_obj[motor_index].channel_a, PWM_PERIOD, -PWM_PERIOD / 100 * percentage);
        rt_pwm_set(dc_motor_cfg_obj[motor_index].pwm_b_dev, dc_motor_cfg_obj[motor_index].channel_b, PWM_PERIOD, 0);
	#endif
    }
    return RT_EOK;
}
