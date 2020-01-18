/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     Soil_L       The first version
 */
#include "can_motor.h"
 
#define DBG_SECTION_NAME  "can_motor"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>


static rt_err_t can_motor_enable(void *mot)
{
    can_motor_t mot_sub = (can_motor_t)mot;
    
    //Avoid repeated opening 
    if(!(mot_sub->can_dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
      rt_err_t res = rt_device_open(mot_sub->can_dev,RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX );
      res = rt_device_control(mot_sub->can_dev,RT_CAN_CMD_SET_BAUD,(void *)mot_sub->Baud);
      res = rt_device_control(mot_sub->can_dev,RT_CAN_CMD_SET_FILTER,&mot_sub->cfg);
    }
    return RT_EOK;
}

static rt_err_t can_motor_disable(void *mot)
{
    can_motor_t mot_sub = (can_motor_t)mot;
    
    rt_err_t res = rt_device_close(mot_sub->can_dev);
    
    return RT_EOK;
}

static rt_err_t can_motor_reset(void *mot)
{
    return RT_EOK;
}

static rt_err_t can_motor_set_speed(void *mot, rt_int16_t thousands)
{
    can_motor_t mot_sub = (can_motor_t)mot;
    
    if(mot_sub->min_num > thousands)
    {
      thousands = mot_sub->min_num;
    }
    else if(mot_sub->max_num < thousands)
    {
      thousands = mot_sub->max_num;
    }
    
    mot_sub->msg->data[(mot_sub->mot_id -1)*2] = thousands>>8;
    mot_sub->msg->data[(mot_sub->mot_id -1)*2+1] = thousands;
    rt_size_t  size = rt_device_write(mot_sub->can_dev, 0, mot_sub->msg, sizeof(*(mot_sub->msg)) );
    
    return RT_EOK;
}


can_motor_t can_motor_create(char *can,struct rt_can_filter_config cfg,rt_uint32_t Baud,rt_can_msg_t msg,rt_int16_t max_num,rt_int16_t min_num,rt_uint8_t mot_id)
{
    RT_ASSERT(max_num>min_num)
    
    can_motor_t new_motor = (can_motor_t)motor_create(sizeof(struct can_motor));
    if (new_motor == RT_NULL)
    {
        return RT_NULL;
    }
    
    new_motor->can_dev = (rt_device_t)rt_device_find(can);
    if (new_motor->can_dev == RT_NULL)
    {
      rt_free(new_motor);
      LOG_E("Falied to find device on %s", can);
      return RT_NULL;
    }
    
    new_motor->cfg = cfg;
    new_motor->Baud = Baud;
    new_motor->msg = msg;
    new_motor->max_num = max_num;
    new_motor->min_num = min_num;
    new_motor->mot_id = mot_id;
    new_motor->mot.enable = can_motor_enable;
    new_motor->mot.set_speed = can_motor_set_speed;
    new_motor->mot.reset = can_motor_reset;
    new_motor->mot.disable = can_motor_disable;
    
    return new_motor;
}
