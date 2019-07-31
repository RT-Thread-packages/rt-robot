#include "ps2.h"

#define PS2_TABLE_SIZE      16
#define PS2_DEFAULT_TABLE                \
{                                        \
    {PS2_BTN_SELECT   ,COMMAND_CAR_STOP},    \
    {PS2_BTN_L3       ,COMMAND_NONE},    \
    {PS2_BTN_R3       ,COMMAND_NONE},    \
    {PS2_BTN_START    ,COMMAND_NONE},    \
    {PS2_BTN_UP       ,COMMAND_CAR_FORWARD},    \
    {PS2_BTN_RIGHT    ,COMMAND_CAR_TURNRIGHT},    \
    {PS2_BTN_DOWN     ,COMMAND_CAR_BACKWARD},    \
    {PS2_BTN_LEFT     ,COMMAND_CAR_TURNLEFT},    \
    {PS2_BTN_L2       ,COMMAND_NONE},    \
    {PS2_BTN_R2       ,COMMAND_NONE},    \
    {PS2_BTN_L1       ,COMMAND_NONE},    \
    {PS2_BTN_R1       ,COMMAND_NONE},    \
    {PS2_BTN_TRIANGLE ,COMMAND_NONE},    \
    {PS2_BTN_CICLE    ,COMMAND_NONE},    \
    {PS2_BTN_FORK     ,COMMAND_NONE},    \
    {PS2_BTN_SQUARE   ,COMMAND_NONE}     \
}

struct ps2_table
{
    int ps2_cmd;
    int standard_cmd;
};

