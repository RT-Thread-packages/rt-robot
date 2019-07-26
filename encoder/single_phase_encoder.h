#ifndef __SINGLE_PHASE_ENCODER_H__
#define __SINGLE_PHASE_ENCODER_H__

#include "encoder.h"

struct single_phase_encoder;
typedef struct single_phase_encoder *single_phase_encoder_t;

single_phase_encoder_t single_phase_encoder_create(rt_base_t pin, rt_uint16_t pulse_revol);

#endif // __SINGLE_PHASE_ENCODER_H__
