#ifndef __DC_MOTOR_H__
#define __DC_MOTOR_H__

#include <rtthread.h>
#include <rtdevice.h>
#include "dc_motor_cfg.h"

int dc_motor_creat(int motor_index);
int dc_motor_enable(int motor_index);
int dc_motor_disable(int motor_index);
int dc_motor_set_speed(int motor_index,rt_int8_t percentage);

#endif // __DC_MOTOR_H__
