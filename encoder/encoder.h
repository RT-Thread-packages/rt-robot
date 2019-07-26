#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <rtthread.h>
#include <rtdevice.h>

enum encoder_direction
{
    ENCODER_DIR_FORWARD,
    ENCODER_DIR_BACKWARD,
};

typedef struct encoder *encoder_t;

struct encoder
{
    rt_int32_t              pulse_count;    /* absolute pulse number in total */
    rt_uint16_t             pulse_revol;    /* pulse number per revolution */
    rt_uint16_t             sample_time;    /* sample time in microseconds for speed measurement */
    rt_tick_t               last_time;
    rt_int32_t              last_count;
    rt_int16_t              cps;
    enum encoder_direction  dir;
    rt_err_t                (*enable)(void *enc);
    rt_err_t                (*disable)(void *enc);
    rt_err_t                (*destroy)(void *enc);
};

encoder_t       encoder_create(rt_size_t size);
rt_err_t        encoder_destroy(encoder_t enc);
rt_err_t        encoder_enable(encoder_t enc);                                      /* start measurement */
rt_err_t        encoder_disable(encoder_t enc);                                     /* stop measurement */
rt_err_t        encoder_reset(encoder_t enc);                                       /* set pulse_count to 0 */

rt_err_t        encoder_set_sample_time(encoder_t enc, rt_uint16_t sample_time);    /* set sample time */

/** rpm = pulse_count / sample_time(ms) / pulse_revol * 1000 * 60 **/
rt_int16_t      encoder_read(encoder_t enc);                                        /* return pulse_count */
rt_int16_t      encoder_measure_cps(encoder_t enc);                                 /* pulse_count per second */
rt_int16_t      encoder_measure_rpm(encoder_t enc);                                 /* revolutions per minute */

#endif
