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
#include "ps2_table.h"

#define DBG_SECTION_NAME  "ps2"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define THREAD_DELAY_TIME           30

#define THREAD_PRIORITY             ((RT_THREAD_PRIORITY_MAX / 3) + 3)
#define THREAD_STACK_SIZE           1024
#define THREAD_TIMESLICE            10

#define KEEP_TIME()                 _delay_us(16);

static uint8_t light_mode = PS2_NO_MODE;
static rt_thread_t tid_ps2 = RT_NULL;
static struct ps2_table table[PS2_TABLE_SIZE] = PS2_DEFAULT_TABLE;

static rt_base_t ps2_cs_pin; 
static rt_base_t ps2_clk_pin;
static rt_base_t ps2_do_pin; 
static rt_base_t ps2_di_pin;

static void hal_cs_high(void)
{
    rt_pin_write(ps2_cs_pin, PIN_HIGH);
}
static void hal_cs_low(void)
{
    rt_pin_write(ps2_cs_pin, PIN_LOW);
}
static void hal_clk_high(void)
{
    rt_pin_write(ps2_clk_pin, PIN_HIGH);
}
static void hal_clk_low(void)
{
    rt_pin_write(ps2_clk_pin, PIN_LOW);
}
static void hal_do_high(void)
{
    rt_pin_write(ps2_do_pin, PIN_HIGH);
}
static void hal_do_low(void)
{
    rt_pin_write(ps2_do_pin, PIN_LOW);
}
static int hal_read_di(void)
{
    return rt_pin_read(ps2_di_pin);
}

static void _delay_us(uint16_t us)
{
    for (int i = 0; i < us; i++)
    {
        for (int j = 0; j < 0x1F;)
            j++;
    }
}

static uint8_t _transfer(uint8_t data)
{
    uint8_t temp = 0;

    for (uint16_t i = 0x01; i < 0x0100; i <<= 1)
    {
        if (i & data)
            hal_do_high(); 
        else
            hal_do_low();

        hal_clk_high();
        KEEP_TIME();
        hal_clk_low();
        if (hal_read_di())
            temp = i | temp;
        KEEP_TIME();
        hal_clk_high();
    }
    
    return temp;
}

static void transfer(const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len)
{
    hal_cs_low();
    _delay_us(16);
    for (uint8_t i = 0; i < len; i++)
    {
        pb_recv[i] = _transfer(pb_send[i]);
    }
    hal_cs_high();
    _delay_us(16);
}

int ps2_scan(ps2_ctrl_data_t pt)
{
    uint8_t temp[9] = {0};

    temp[0] = 0x01;
    temp[1] = 0x42;
    temp[3] = 0;
    temp[4] = 0;

    transfer(temp, temp, 9);

    pt->button = temp[3] | (temp[4] << 8);
    pt->right_stick_x = temp[5];
    pt->right_stick_y = temp[6];
    pt->left_stick_x = temp[7];
    pt->left_stick_y = temp[8];

    if (temp[2] == 0x5A)
    {
        light_mode = temp[1];
        return 1;
    }
    else
    {
        light_mode = PS2_NO_MODE;
    }

    return 0;
}

/**
 * @return PS2_GREEN_MDOE or PS2_RED_MDOE or other(no connect) 
 */
int ps2_read_light(void)
{
    return light_mode;
}

static rt_err_t ps2_sender_send(rt_uint16_t cmd, void *param, rt_uint16_t size)
{
    // TODO

    if (cmd == COMMAND_RC_VIBRATE)
    {

    }
    else
    {
        return RT_ERROR;
    }

    return RT_EOK;
}

static struct command_sender ps2_sender = {
    .name = "ano",
    .send = ps2_sender_send
};

command_sender_t ps2_get_sender(void)
{
    return &ps2_sender;
}

static void ps2_thread_entry(void *param)
{
    struct ps2_ctrl_data ctrl_data;
    struct cmd_velocity target_velocity;

    while (1)
    {
        rt_thread_mdelay(THREAD_DELAY_TIME);   
        ps2_scan(&ctrl_data);
        // look-up table and send standard command
        for (int i = 0; i < PS2_TABLE_SIZE; i++)
        {
            if (!(ctrl_data.button & table[i].ps2_cmd))
            {
                if (table[i].standard_cmd != COMMAND_NONE)
                {
                    command_handle(table[i].standard_cmd, RT_NULL, 0);
                }
            }
        }

        // rocker
        if (ps2_read_light() == PS2_RED_MODE)
        {
            uint8_t value[4] = {
                ctrl_data.left_stick_x, 
                ctrl_data.right_stick_x,
                ctrl_data.left_stick_y,
                ctrl_data.right_stick_y};

            rt_int16_t cmd[4] = {
                table[PS2_ROCKER_LX].standard_cmd,
                table[PS2_ROCKER_LY].standard_cmd,
                table[PS2_ROCKER_RX].standard_cmd,
                table[PS2_ROCKER_RY].standard_cmd};

            for (int i = 0; i < 4; i++)
            {
                if (cmd[i] != COMMAND_NONE)
                {
                    float tmp = CHASSIS_VELOCITY_LINEAR_MAXIMUM;
                    if (cmd[i] == COMMAND_SET_CHASSIS_VELOCITY_ANGULAR_Z)
                    {
                        tmp = CHASSIS_VELOCITY_ANGULAR_MAXIMUM;
                    }
                    
                    target_velocity.data.common = tmp * ((0x80 - (int)(i/2)) - value[i]) / 128;
                    command_handle(cmd[i], &target_velocity, sizeof(struct cmd_velocity));
                }
            }
        }
    }
}

void ps2_init(rt_base_t cs_pin, rt_base_t clk_pin, rt_base_t do_pin, rt_base_t di_pin)
{
    ps2_cs_pin = cs_pin;
    ps2_clk_pin = clk_pin;
    ps2_do_pin = do_pin;
    ps2_di_pin = di_pin;

    rt_pin_mode(ps2_cs_pin,  PIN_MODE_OUTPUT);
    rt_pin_mode(ps2_clk_pin, PIN_MODE_OUTPUT);
    rt_pin_mode(ps2_do_pin,  PIN_MODE_OUTPUT);
    rt_pin_mode(ps2_di_pin,  PIN_MODE_INPUT);
    
    hal_cs_high();
    hal_clk_high();

    // cmd_info.target = target;

    tid_ps2 = rt_thread_create("ps2",
                          ps2_thread_entry, RT_NULL,
                          THREAD_STACK_SIZE,
                          THREAD_PRIORITY, THREAD_TIMESLICE);

    if (tid_ps2 != RT_NULL)
    {
        rt_thread_startup(tid_ps2);
    }
    else
    {
        LOG_E("Can't create thread for ps2");
    }
}
