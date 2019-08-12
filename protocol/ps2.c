/*********************************************************
Change From YFRobot. www.yfrobot.com
**********************************************************/

#include "ps2.h"
#include "ps2_table.h"

#define DBG_SECTION_NAME  "ps2"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define THREAD_DELAY_TIME           30

#define THREAD_PRIORITY             ((RT_THREAD_PRIORITY_MAX / 3) + 2)
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

static struct command_info cmd_info;

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

static void ps2_thread_entry(void *param)
{
    struct ps2_ctrl_data ctrl_data;
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
                    cmd_info.cmd = table[i].standard_cmd;
                    command_send(&cmd_info);
                }
            }
        }

        // rocker
        if (ps2_read_light() == PS2_RED_MODE)
        {
            if (table[PS2_ROCKER_LX].standard_cmd != COMMAND_NONE)
            {
                cmd_info.cmd = table[PS2_ROCKER_LX].standard_cmd;
                cmd_info.param = &ctrl_data.left_stick_x;
                command_send(&cmd_info);
            }
            if (table[PS2_ROCKER_LY].standard_cmd != COMMAND_NONE)
            {
                cmd_info.cmd = table[PS2_ROCKER_LY].standard_cmd;
                cmd_info.param = &ctrl_data.left_stick_y;
                command_send(&cmd_info);
            }
            if (table[PS2_ROCKER_RX].standard_cmd != COMMAND_NONE)
            {
                cmd_info.cmd = table[PS2_ROCKER_RX].standard_cmd;
                cmd_info.param = &ctrl_data.right_stick_x;
                command_send(&cmd_info);
            }
            if (table[PS2_ROCKER_RY].standard_cmd != COMMAND_NONE)
            {
                cmd_info.cmd = table[PS2_ROCKER_RY].standard_cmd;
                cmd_info.param = &ctrl_data.right_stick_y;
                command_send(&cmd_info);
            }
        }
    }
}

void ps2_init(rt_base_t cs_pin, rt_base_t clk_pin, rt_base_t do_pin, rt_base_t di_pin, void *target)
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

    cmd_info.target = target;

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
