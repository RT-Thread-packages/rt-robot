/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#include "ps2.h"

#define PS2_TABLE_SIZE      20
#define PS2_DEFAULT_TABLE                \
{                                        \
    {PS2_BTN_SELECT   ,COMMAND_SET_CHASSIS_STOP},    \
    {PS2_BTN_L3       ,COMMAND_NONE},    \
    {PS2_BTN_R3       ,COMMAND_NONE},    \
    {PS2_BTN_START    ,COMMAND_NONE},    \
    {PS2_BTN_UP       ,COMMAND_SET_CHASSIS_FORWARD},    \
    {PS2_BTN_RIGHT    ,COMMAND_SET_CHASSIS_ROTATE_RIGHT},    \
    {PS2_BTN_DOWN     ,COMMAND_SET_CHASSIS_BACKWARD},    \
    {PS2_BTN_LEFT     ,COMMAND_SET_CHASSIS_ROTATE_LEFT},    \
    {PS2_BTN_L2       ,COMMAND_NONE},    \
    {PS2_BTN_R2       ,COMMAND_NONE},    \
    {PS2_BTN_L1       ,COMMAND_SET_CHASSIS_MOVE_LEFT},    \
    {PS2_BTN_R1       ,COMMAND_SET_CHASSIS_MOVE_RIGHT},    \
    {PS2_BTN_TRIANGLE ,COMMAND_NONE},    \
    {PS2_BTN_CICLE    ,COMMAND_NONE},    \
    {PS2_BTN_FORK     ,COMMAND_NONE},    \
    {PS2_BTN_SQUARE   ,COMMAND_NONE},    \
    {PS2_ROCKER_LX    ,COMMAND_SET_CHASSIS_VELOCITY_ANGULAR_Z},    \
    {PS2_ROCKER_LY    ,COMMAND_SET_CHASSIS_VELOCITY_LINEAR_X},    \
    {PS2_ROCKER_RX    ,COMMAND_SET_CHASSIS_VELOCITY_LINEAR_Y},    \
    {PS2_ROCKER_RY    ,COMMAND_NONE}     \
}

struct ps2_table
{
    int ps2_cmd;
    int standard_cmd;
};
