/*********************************************************
Change From YFRobot. www.yfrobot.com
**********************************************************/

#include "ps2.h"
#include "ps2_table.h"

#define DBG_SECTION_NAME  "ps2"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

#define HAL_CS_H()                  rt_pin_write(PS2_DEFAULT_CS_PIN, PIN_HIGH)
#define HAL_CS_L()                  rt_pin_write(PS2_DEFAULT_CS_PIN, PIN_LOW)
#define HAL_CLK_H()                 rt_pin_write(PS2_DEFAULT_CLK_PIN, PIN_HIGH)
#define HAL_CLK_L()                 rt_pin_write(PS2_DEFAULT_CLK_PIN, PIN_LOW)
#define HAL_DO_H()                  rt_pin_write(PS2_DEFAULT_DO_PIN, PIN_HIGH)
#define HAL_DO_L()                  rt_pin_write(PS2_DEFAULT_DO_PIN, PIN_LOW)
#define HAL_GET_DI()                rt_pin_read(PS2_DEFAULT_DI_PIN)

#define THREAD_DELAY_TIME           50

#define THREAD_PRIORITY             16
#define THREAD_STACK_SIZE           1024
#define THREAD_TIMESLICE            10

#define KEEP_TIME()                 _delay_us(16);

static uint8_t light_mode = PS2_NO_MODE;
static rt_thread_t tid_ps2 = RT_NULL;
static struct ps2_table table[PS2_TABLE_SIZE] = PS2_DEFAULT_TABLE;

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
            HAL_DO_H(); 
        else
            HAL_DO_L();

        HAL_CLK_H();
        KEEP_TIME();
        HAL_CLK_L();
        if (HAL_GET_DI())
            temp = i | temp;
        KEEP_TIME();
        HAL_CLK_H();
    }
    
    return temp;
}

static void transfer(const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len)
{
    HAL_CS_L();
    _delay_us(16);
    for (uint8_t i = 0; i < len; i++)
    {
        pb_recv[i] = _transfer(pb_send[i]);
    }
    HAL_CS_H();
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
                    command_handle(table[i].standard_cmd, RT_NULL);
                }
            }
        }
    }
}

void ps2_init(void)
{
    rt_pin_mode(PS2_DEFAULT_CS_PIN,  PIN_MODE_OUTPUT);
    rt_pin_mode(PS2_DEFAULT_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(PS2_DEFAULT_DO_PIN,  PIN_MODE_OUTPUT);
    rt_pin_mode(PS2_DEFAULT_DI_PIN,  PIN_MODE_INPUT);

    HAL_CS_H();
    HAL_CLK_H();

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
