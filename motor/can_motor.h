/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     Soil_L       The first version
 */
#ifndef _CAN_MOTOR_H_
#define _CAN_MOTOR_H_

#include "motor.h"

struct can_motor
{
    struct motor                mot;
    rt_device_t                 can_dev;
		struct rt_can_filter_config cfg;
	  rt_uint32_t                 Baud;
		rt_can_msg_t                msg;  //the pointer of message defined by the users,it decide the number of the can motor in the can net
		rt_int16_t                  max_num;
		rt_int16_t                  min_num;
		rt_uint8_t                  mot_id;	//the id of the can motor in the can net
};
typedef struct can_motor *can_motor_t;

can_motor_t can_motor_create(char *can,struct rt_can_filter_config cfg,rt_uint32_t Baud,rt_can_msg_t msg,rt_int16_t max_num,rt_int16_t min_num,rt_uint8_t mot_id);

#endif // __CAN_MOTOR_H__
