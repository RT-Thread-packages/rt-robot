/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-14     Soil_L       The first version
 */
#include "can_encoder.h"
 
#define DBG_SECTION_NAME  "can_encoder"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

rt_sem_t rx_sem_g;

rt_err_t encoder_isr(rt_device_t dev, rt_size_t size)
{
    rt_sem_release(rx_sem_g);
    return RT_EOK;
}

static rt_err_t can_encoder_enable(void *enc)
{
    can_encoder_t enc_sub = (can_encoder_t)enc;
    
    volatile rt_err_t res;
    
    //Avoid repeated opening 
    if(!(enc_sub->can_dev->flag & RT_DEVICE_FLAG_ACTIVATED))
    {
        res = rt_device_open(enc_sub->can_dev,RT_DEVICE_FLAG_INT_RX|RT_DEVICE_FLAG_INT_TX);
        res = rt_device_control(enc_sub->can_dev,RT_CAN_CMD_SET_BAUD,(void *)enc_sub->Baud);
        res = rt_device_control(enc_sub->can_dev,RT_CAN_CMD_SET_FILTER,&enc_sub->cfg);
    }
    res = rt_device_set_rx_indicate(enc_sub->can_dev,encoder_isr);
    
    return RT_EOK;
}

static rt_err_t can_encoder_disable(void *enc)
{
    return RT_EOK;
}

static rt_err_t can_encoder_destroy(void *enc)
{
    can_encoder_t enc_sub = (can_encoder_t)enc;
    
    volatile rt_err_t res;
    res = rt_device_close(enc_sub->can_dev);
    rt_sem_delete(*(enc_sub->rx_sem));
    rt_free(enc_sub);
    
    return RT_EOK;
}

can_encoder_t can_encoder_create(char *can,struct rt_can_filter_config cfg,rt_uint32_t Baud,rt_sem_t* rx_sem)
{
    can_encoder_t new_encoder = (can_encoder_t)encoder_create(sizeof(struct can_encoder),0);
    if(new_encoder == RT_NULL)
    {
        return RT_NULL;
    }
    
    new_encoder->can_dev = (rt_device_t)rt_device_find(can);
    if (new_encoder->can_dev == RT_NULL)
    {
        rt_free(new_encoder);
        LOG_E("Falied to find device on %s", can);
        return RT_NULL;
    }
    
    new_encoder->cfg = cfg;
    new_encoder->enc.enable = can_encoder_enable;
    new_encoder->enc.disable = can_encoder_disable;
    new_encoder->enc.destroy = can_encoder_destroy;
    new_encoder->Baud = Baud;
    new_encoder->rx_sem = rx_sem;
    rx_sem_g = *rx_sem;
    return new_encoder;
}
