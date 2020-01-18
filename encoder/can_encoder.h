/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     Soil_L       The first version
 */
#ifndef _CAN_ENCODER_
#define _CAN_ENCODER_

#include "encoder.h"

struct can_encoder
{
	struct encoder              enc;
	rt_device_t                 can_dev;
	struct rt_can_filter_config cfg;
	rt_uint32_t                 Baud;
	rt_sem_t                    *rx_sem;
	rt_uint32_t                 rpm;
};
typedef struct can_encoder *can_encoder_t;

can_encoder_t can_encoder_create(char *can,struct rt_can_filter_config cfg,rt_uint32_t Baud,rt_sem_t* rx_sem);

#endif  //CAN_ENCODER_
