/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __AB_PHASE_ENCODER_H__
#define __AB_PHASE_ENCODER_H__

#include "encoder.h"

struct ab_phase_encoder;
typedef struct ab_phase_encoder *ab_phase_encoder_t;

ab_phase_encoder_t ab_phase_encoder_create(rt_base_t pin_a, rt_base_t pin_b, rt_uint16_t pulse_revol, rt_uint16_t sample_time);

#endif // __AB_PHASE_ENCODER_H__
