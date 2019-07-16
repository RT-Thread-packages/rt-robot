#include <rtdevice.h>
#include <encoder.h>

#define DBG_SECTION_NAME  "encoder"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

void encoder_isr(void *args)
{
    rt_uint16_t* pulse_count = (rt_uint16_t*)args;
    (*pulse_count)++;
    // LOG_D("Count %d", *pulse_count);
}

encoder_t encoder_create(rt_base_t pin, rt_uint16_t pulse_revol)
{
    // Malloc memory and initialize pulse_count and pin
    encoder_t new_encoder = (encoder_t)rt_malloc(sizeof(struct encoder));
    if(new_encoder == RT_NULL)
    {
        LOG_E("Failed to malloc memory for new encoder\n");
        return RT_NULL;
    }

    new_encoder->pulse_count = 0;
    new_encoder->pin = pin;
    new_encoder->pulse_revol = pulse_revol;
    new_encoder->last_count = 0;

    return new_encoder;
}

void encoder_destroy(encoder_t enc)
{
    LOG_D("Free Encoder");

    rt_free(enc);
}

rt_err_t encoder_enable(encoder_t enc)
{
    LOG_D("Enabling encoder");

    // Attach interrupts
    rt_pin_mode(enc->pin, PIN_MODE_INPUT_PULLUP);
    rt_pin_attach_irq(enc->pin, PIN_IRQ_MODE_FALLING, encoder_isr, &(enc->pulse_count));
    enc->last_time = rt_tick_get();

    return rt_pin_irq_enable(enc->pin, PIN_IRQ_ENABLE);
}

rt_err_t encoder_disable(encoder_t enc)
{
    LOG_D("Diabling encoder");

    return rt_pin_detach_irq(enc->pin);
}

rt_int16_t encoder_read(encoder_t enc)
{
    return enc->pulse_count;
}

rt_err_t encoder_reset(encoder_t enc)
{
    enc -> pulse_count = 0;
    return RT_EOK;
}

rt_int16_t encoder_measure_cps(encoder_t enc)
{
    // TODO
    // return count per second
    if((rt_tick_get() - enc->last_time) < rt_tick_from_millisecond(enc->sample_time))
    {
        // LOG_D("Encoder waiting ... ");
        // LOG_D("Current count per second %d", enc->cps);
        return enc->cps;
    }

    enc->cps = (enc->pulse_count - enc->last_count) * 1000 / enc->sample_time;
    enc->last_count = enc->pulse_count;
    enc->last_time = rt_tick_get();
    // LOG_D("Current count per second %d", enc->cps);

    return enc->cps;
}

rt_int16_t encoder_measure_rpm(encoder_t enc)
{
    // return resolution per minute
    // LOG_D("Current count per revolution %d",  enc->pulse_revol);

    rt_int16_t res_rpm = encoder_measure_cps(enc) * 60 / enc->pulse_revol;
    // LOG_D("Current revol per minute %d", res_rpm);
    return res_rpm;
}

void encoder_set_sample_time(encoder_t enc, rt_uint16_t sample_time)
{
    enc->sample_time = sample_time;
}
