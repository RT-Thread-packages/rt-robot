/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-17     Wu Han       The first version
 */

#ifndef __SINGLE_PHASE_ENCODER_H__
#define __SINGLE_PHASE_ENCODER_H__

#include "encoder.h"

struct single_phase_encoder;
typedef struct single_phase_encoder *single_phase_encoder_t;

single_phase_encoder_t single_phase_encoder_create(rt_base_t pin, rt_uint16_t pulse_revol, rt_uint16_t sample_time);

#endif // __SINGLE_PHASE_ENCODER_H__
