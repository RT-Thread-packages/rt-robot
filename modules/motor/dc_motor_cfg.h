#ifndef __DC_MOTOR_CFG_H__
#define __DC_MOTOR_CFG_H__
#include <rtthread.h>
#include <rtdevice.h>

/* PWM mode
	ONE_PWM_DUAL_PIN  one pwm and two pin ,eg. TB6612
	DUAL_PWM		  two pwm ,eg. L9110
*/
#define ONE_PWM_DUAL_PIN
//#define DUAL_PWM

/* the period of timer
	PWM_PERIOD 
*/
#define PWM_PERIOD 50000

/* the number of dc_motor 
	NOTE:if the number of dc_motor over 4,please increase the DC_MOTOR_PWM_CFG
*/
#define DC_MOTOR_NUMBER	4

/* the dc motor pwm config*/
#ifdef	ONE_PWM_DUAL_PIN
#if 	DC_MOTOR_NUMBER > 0
	#define DC_MOTOR_PWM_CFG1				\
		{									\
			.pwm_name	=	"pwm8",			\
			.channel	=    	1,			\
			.pina_index	=		24,			\
			.pinb_index	=		25,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 1
	#define DC_MOTOR_PWM_CFG2				\
		{									\
			.pwm_name	=	"pwm8",			\
			.channel	=    	1,			\
			.pina_index	=		24,			\
			.pinb_index	=		25,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 2
	#define DC_MOTOR_PWM_CFG3				\
		{									\
			.pwm_name	=	"pwm8",			\
			.channel	=    	1,			\
			.pina_index	=		24,			\
			.pinb_index	=		25,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 3
	#define DC_MOTOR_PWM_CFG4				\
		{									\
			.pwm_name	=	"pwm8",			\
			.channel	=    	1,			\
			.pina_index	=		24,			\
			.pinb_index	=		25,			\
		}
#endif
#endif	/* ONE_PWM_DUAL_PIN*/
			
#ifdef	DUAL_PWM
#if 	DC_MOTOR_NUMBER > 0
	#define DC_MOTOR_PWM_CFG1				\
		{									\
			.pwm_a		=	"pwm8",			\
			.channel_a	=    	1,			\
			.pwm_b		=	"pwm8",			\
			.channel_b	=		2,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 1
	#define DC_MOTOR_PWM_CFG2				\
		{									\
			.pwm_a		=	"pwm8",			\
			.channel_a	=    	3,			\
			.pwm_b		=	"pwm8",			\
			.channel_b	=		4,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 2
	#define DC_MOTOR_PWM_CFG3				\
		{									\
			.pwm_a		=	"pwm4",			\
			.channel_a	=    	1,			\
			.pwm_b		=	"pwm4",			\
			.channel_b	=		2,			\
		}
#endif
#if 	DC_MOTOR_NUMBER > 3
	#define DC_MOTOR_PWM_CFG4				\
		{									\
			.pwm_a		=	"pwm4",			\
			.channel_a	=    	3,			\
			.pwm_b		=	"pwm4",			\
			.channel_b	=		4,			\
		}
#endif			
#endif /* DUAL_PWM*/

#endif // __DC_MOTOR_CFG_H__
