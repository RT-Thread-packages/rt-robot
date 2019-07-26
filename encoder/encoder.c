#include <rtdevice.h>
#include <encoder.h>

#define DBG_SECTION_NAME  "encoder"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

encoder_t encoder_create(rt_size_t size)
{
    // Malloc memory and initialize
    encoder_t new_encoder = (encoder_t)rt_malloc(size);
    if(new_encoder == RT_NULL)
    {
        LOG_E("Failed to malloc memory for new encoder\n");
        return RT_NULL;
    }

    new_encoder->pulse_count = 0;
    new_encoder->last_count = 0;

    return new_encoder;
}

rt_err_t encoder_destroy(encoder_t enc)
{
    LOG_D("Free Encoder");
    RT_ASSERT(enc != RT_NULL);

    return enc->destroy(enc);
}

rt_err_t encoder_enable(encoder_t enc)
{
    LOG_D("Enabling encoder");
    RT_ASSERT(enc != RT_NULL);

    return enc->enable(enc);
}

rt_err_t encoder_disable(encoder_t enc)
{
    LOG_D("Diabling encoder");
    RT_ASSERT(enc != RT_NULL);

    return enc->disable(enc);
}

rt_int16_t encoder_read(encoder_t enc)
{
    RT_ASSERT(enc != RT_NULL);

    return enc->pulse_count;
}

rt_err_t encoder_reset(encoder_t enc)
{
    RT_ASSERT(enc != RT_NULL);

    enc->pulse_count = 0;

    return RT_EOK;
}

rt_int16_t encoder_measure_cps(encoder_t enc)
{
    RT_ASSERT(enc != RT_NULL);
    // TODO
    // return count per second
    if((rt_tick_get() - enc->last_time) < rt_tick_from_millisecond(enc->sample_time))
    {
        LOG_D("Encoder waiting ... ");
        return enc->cps;
    }

    rt_int32_t diff_count = enc->pulse_count - enc->last_count;
    
    // if (diff_count >= INT32_MAX)
    // {
    //     diff_count = -(enc->pulse_count + enc->last_count);
    // }
    // if (diff_count <= INT32_MIN)
    // {
    //     diff_count = enc->pulse_count + enc->last_count;
    // }

    enc->cps = diff_count * 1000 / enc->sample_time;
    enc->last_count = enc->pulse_count;
    enc->last_time = rt_tick_get();

    return enc->cps;
}

rt_int16_t encoder_measure_rpm(encoder_t enc)
{
    RT_ASSERT(enc != RT_NULL);
    
    // return resolution per minute
    rt_int16_t res_rpm = encoder_measure_cps(enc) * 60 / enc->pulse_revol;

    return res_rpm;
}

rt_err_t encoder_set_sample_time(encoder_t enc, rt_uint16_t sample_time)
{
    RT_ASSERT(enc != RT_NULL);

    enc->sample_time = sample_time;

    return RT_EOK;
}
