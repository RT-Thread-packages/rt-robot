#ifndef __AB_PHASE_ENCODER_H__
#define __AB_PHASE_ENCODER_H__

#include "encoder.h"

struct ab_phase_encoder
{
    struct encoder  enc;
    rt_base_t       pin_a;            /* interrupt pin  */
    rt_base_t       pin_b;
    rt_int16_t      last_value;
};

typedef struct ab_phase_encoder *ab_phase_encoder_t;

ab_phase_encoder_t ab_phase_encoder_create(rt_base_t pin_a, rt_base_t pin_b, rt_uint16_t pulse_revol);

#endif // __AB_PHASE_ENCODER_H__
