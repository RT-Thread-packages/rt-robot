/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "ab_phase_encoder.h"

#define DBG_SECTION_NAME  "ab_phase_encoder"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

struct ab_phase_encoder
{
    struct encoder  enc;
    rt_base_t       pin_a;            /* interrupt pin  */
    rt_base_t       pin_b;
    rt_int16_t      last_value;
};

static const uint8_t AB_PHASE_ENCODER_TABLE[4] = {2, 0, 3, 1};

static void encoder_isr(void *args)
{
    ab_phase_encoder_t enc_sub = (ab_phase_encoder_t)args;
    int val = rt_pin_read(enc_sub->pin_a) * 2 + rt_pin_read(enc_sub->pin_b);

    if (AB_PHASE_ENCODER_TABLE[enc_sub->last_value] == val)
    {
        enc_sub->enc.dir = ENCODER_DIR_FORWARD;
        enc_sub->enc.pulse_count++;
    }
    else
    {
        enc_sub->enc.dir = ENCODER_DIR_BACKWARD;
        enc_sub->enc.pulse_count--;
    }
    enc_sub->last_value = val;
}

static rt_err_t ab_phase_encoder_enable(void *enc)
{
    ab_phase_encoder_t enc_sub = (ab_phase_encoder_t)enc;

    // Attach interrupts
    rt_pin_mode(enc_sub->pin_a, PIN_MODE_INPUT);
    rt_pin_mode(enc_sub->pin_b, PIN_MODE_INPUT);
    rt_pin_attach_irq(enc_sub->pin_a, PIN_IRQ_MODE_RISING_FALLING, encoder_isr, enc_sub);
    rt_pin_attach_irq(enc_sub->pin_b, PIN_IRQ_MODE_RISING_FALLING, encoder_isr, enc_sub);
    enc_sub->last_value = rt_pin_read(enc_sub->pin_a) * 2 + rt_pin_read(enc_sub->pin_b);
    enc_sub->enc.last_time = rt_tick_get();
    rt_pin_irq_enable(enc_sub->pin_a, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(enc_sub->pin_b, PIN_IRQ_ENABLE);

    return RT_EOK;
}

static rt_err_t ab_phase_encoder_disable(void *enc)
{
    ab_phase_encoder_t enc_sub = (ab_phase_encoder_t)enc;

    rt_pin_irq_enable(enc_sub->pin_a, PIN_IRQ_DISABLE);
    rt_pin_irq_enable(enc_sub->pin_b, PIN_IRQ_DISABLE);

    return RT_EOK;
}

static rt_err_t ab_phase_encoder_destroy(void *enc)
{
    ab_phase_encoder_disable(enc);
    ab_phase_encoder_t enc_sub = (ab_phase_encoder_t)enc;
    rt_pin_detach_irq(enc_sub->pin_a);
    rt_pin_detach_irq(enc_sub->pin_b);

    rt_free(enc_sub);

    return RT_EOK;
}

ab_phase_encoder_t ab_phase_encoder_create(rt_base_t pin_a, rt_base_t pin_b, rt_uint16_t pulse_revol, rt_uint16_t sample_time)
{
    // Malloc memory and initialize pulse_count and pin
    ab_phase_encoder_t new_encoder = (ab_phase_encoder_t)encoder_create(sizeof(struct ab_phase_encoder), sample_time);
    if(new_encoder == RT_NULL)
    {
        return RT_NULL;
    }
    
    new_encoder->pin_a = pin_a;
    new_encoder->pin_b = pin_b;
    new_encoder->last_value = 0;
    new_encoder->enc.pulse_revol = pulse_revol;
    new_encoder->enc.enable = ab_phase_encoder_enable;
    new_encoder->enc.disable = ab_phase_encoder_disable;
    new_encoder->enc.destroy = ab_phase_encoder_destroy;

    return new_encoder;
}
