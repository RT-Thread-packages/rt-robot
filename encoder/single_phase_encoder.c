/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */

#include "single_phase_encoder.h"

#define DBG_SECTION_NAME  "single_phase_encoder"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

struct single_phase_encoder
{
    struct encoder  enc;
    rt_base_t       pin;            /* interrupt pin  */
};

static void encoder_isr(void *args)
{
    rt_int32_t* pulse_count = (rt_int32_t*)args;
    (*pulse_count)++;
}

static rt_err_t single_phase_encoder_enable(void *enc)
{
    single_phase_encoder_t enc_sub = (single_phase_encoder_t)enc;

    // Attach interrupts
    rt_pin_mode(enc_sub->pin, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(enc_sub->pin, PIN_IRQ_MODE_FALLING, encoder_isr, &(enc_sub->enc.pulse_count));
    enc_sub->enc.last_time = rt_tick_get();

    return rt_pin_irq_enable(enc_sub->pin, PIN_IRQ_ENABLE);
}

static rt_err_t single_phase_encoder_disable(void *enc)
{
    single_phase_encoder_t enc_sub = (single_phase_encoder_t)enc;

    return rt_pin_irq_enable(enc_sub->pin, PIN_IRQ_DISABLE);;
}

static rt_err_t single_phase_encoder_destroy(void *enc)
{
    single_phase_encoder_disable(enc);
    single_phase_encoder_t enc_sub = (single_phase_encoder_t)enc;
    rt_pin_detach_irq(enc_sub->pin);

    rt_free(enc);

    return RT_EOK;
}

single_phase_encoder_t single_phase_encoder_create(rt_base_t pin, rt_uint16_t pulse_revol, rt_uint16_t sample_time)
{
    // Malloc memory and initialize pulse_count and pin
    single_phase_encoder_t new_encoder = (single_phase_encoder_t)encoder_create(sizeof(struct single_phase_encoder), sample_time);
    if(new_encoder == RT_NULL)
    {
        return RT_NULL;
    }

    new_encoder->pin = pin;
    new_encoder->enc.pulse_revol = pulse_revol;
    new_encoder->enc.enable = single_phase_encoder_enable;
    new_encoder->enc.disable = single_phase_encoder_disable;
    new_encoder->enc.destroy = single_phase_encoder_destroy;

    return new_encoder;
}
