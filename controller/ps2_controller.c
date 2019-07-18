/*********************************************************
Change From YFRobot. www.yfrobot.com
**********************************************************/

#include <rtthread.h>
#include <rtdevice.h>
#include "controller.h"
#include "ps2_controller.h"

#define DBG_SECTION_NAME  "PS2"
#define DBG_LEVEL         DBG_LOG
#include <rtdbg.h>

static int ps2_controller_init(ps2_controller_t ps2)
{
    rt_pin_mode(HAL_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_DO_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_DI_PIN, PIN_MODE_INPUT);

    HAL_CS_H();
    HAL_CLK_H();

    return 0;
}

static void ps2_controller_delay(uint16_t us)
{
    // TODO

    for (int i = 0; i < us; i++)
    {
        // maybe the cyclic value needs to be modified
        for (int j = 0; j < 0x1F;)
        {
            j++;
        }
    }
}

static uint8_t ps2_controller_recv_byte(ps2_controller_t ps2)
{
    uint8_t temp = 0;

    for (uint16_t i = 0x01; i < 0x100; i <<= 1)
    {
        HAL_CLK_H();
        KEEP_TIME();
        HAL_CLK_L();
        KEEP_TIME();
        if (HAL_GET_DI())
        {
            temp = i | temp;
        }
        HAL_CLK_H();
        
    }

    return temp;
}

static void ps2_controller_send_byte(ps2_controller_t ps2, uint8_t data)
{
    for (uint16_t i = 0x01; i < 0x0100; i <<= 1)
    {
        if (i & data)
        {
            HAL_DO_H(); 
        }
        else
        {
            HAL_DO_L();
        }

        HAL_CLK_H();
        KEEP_TIME();
        HAL_CLK_L();
        KEEP_TIME();
        HAL_CLK_H();
    }
}

static void ps2_controller_send_bytes(ps2_controller_t ps2, const uint8_t *pb, uint8_t len)
{
    HAL_CS_L();
    ps2_controller_delay(16);
    for (uint8_t i = 0; i < len; i++)
    {
        ps2_controller_send_byte(ps2, pb[i]);
    }
    HAL_CS_H();
    ps2_controller_delay(16);
}

static void ps2_controller_send_recv(ps2_controller_t ps2, const uint8_t *pb_send, uint8_t len_send, uint8_t *pb_recv, uint8_t len_recv)
{
    HAL_CS_L();
    for (uint8_t i = 0; i < len_send; i++)
    {
        ps2_controller_send_byte(ps2, pb_send[i]);
    }
    for (uint8_t i = 0; i < len_recv; i++)
    {
        pb_recv[i] = ps2_controller_recv_byte(ps2);
    }
    HAL_CS_H();
}

static void ps2_controller_transfer(ps2_controller_t ps2, const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len)
{
    // TODO

}

static void ps2_controller_enter_config(ps2_controller_t ps2)
{
    uint8_t temp[9] = {0};

    temp[0] = 0x01;
    temp[1] = 0x43;
    temp[2] = 0x00;
    temp[3] = 0x01;

    temp[4] = 0x00;
    temp[5] = 0x00;
    temp[6] = 0x00;
    temp[7] = 0x00;
    temp[8] = 0x00;

    ps2_controller_send_bytes(ps2, temp, sizeof(temp));
}

static void ps2_controller_exit_config(ps2_controller_t ps2)
{
    uint8_t temp[9] = {0};

    temp[0] = 0x01;
    temp[1] = 0x43;
    temp[2] = 0x00;
    temp[3] = 0x00;
    temp[4] = 0x5A;
    temp[5] = 0x5A;
    temp[6] = 0x5A;
    temp[7] = 0x5A;
    temp[8] = 0x5A;

    ps2_controller_send_bytes(ps2, temp, sizeof(temp));
}

void ps2_controller_open_vibration_mode(ps2_controller_t ps2)
{
    uint8_t temp[5] = {0};

    temp[0] = 0x01;
    temp[1] = 0x4D;
    temp[2] = 0x00;
    temp[3] = 0x00;
    temp[4] = 0x01;

    ps2_controller_send_bytes(ps2, temp, sizeof(temp));
}

/**
 * @brief 控制手柄振动
 * @param s_motor 右侧小振动电机振动幅度, 0x00 关, 其它开？
 * @param l_motor 左侧大振动电机振动幅度, 0x40~0xFF 电机开, 值越大振动越大
 * @note 需先调用 ps2_enter_config
 */
void ps2_controller_vibrate(ps2_controller_t ps2, uint8_t s_motor, uint8_t l_motor)
{
    ps2_controller_enter_config(ps2);
    ps2_controller_open_vibration_mode(ps2);
    ps2_controller_exit_config(ps2);

    uint8_t temp[9];

    temp[0] = 0x01;
    temp[1] = 0x42;
    temp[3] = s_motor;
    temp[4] = l_motor;

    ps2_controller_send_bytes(ps2, temp, sizeof(temp));

}

int ps2_controller_recev(ps2_controller_t ps2, ps2_ctrl_data_t *pt)
{
    uint8_t temp[2] = {0x01, 0x42};
    uint8_t temp_recv[7] = {0x00};

    ps2_controller_send_recv(ps2, temp, 2, temp_recv, 7);

    pt->button = temp_recv[1] | (temp_recv[2] << 8);
    pt->right_stick_x = temp_recv[3];
    pt->right_stick_y = temp_recv[4];
    pt->left_stick_x = temp_recv[5];
    pt->left_stick_y = temp_recv[6];

    // 确认已连接接收器
    if (temp_recv[0] == 0x5A)
    {
        return 1;
    }

    // rt_kprintf("0x%x %d %d \r\n", pt->button, pt->left_stick_x, pt->left_stick_y);

    return 0;
}

static rt_err_t ps2_controller_parse_cmd(controller_t contrl, ps2_ctrl_data_t *ctrl_data)
{
    // 1. Get data
    // 2. Convert to command
    // 3. Send to mailbox

    // rt_int8_t analog;

    // analog = +(100 * (ctrl_data->left_stick_x - 0x80) / 127);
    // controller_adapter_send_cmd(adpt, PS2_STICK_LX, &analog);
    // analog = -(100 * (ctrl_data->left_stick_y - 0x7F) / 127);
    // controller_adapter_send_cmd(adpt, PS2_STICK_LY, &analog);
    // analog = -(100 * (ctrl_data->right_stick_x - 0x80) / 127);
    // controller_adapter_send_cmd(adpt, PS2_STICK_RX, &analog);
    // analog = +(100 * (ctrl_data->right_stick_y - 0x7F) / 127);
    // controller_adapter_send_cmd(adpt, PS2_STICK_RY, &analog);

    // if (!(ctrl_data->button & PS2_BTN_SELECT))
    // {
    //     controller_adapter_send_cmd(adpt, PS2_BTN_SELECT, RT_NULL);
    // }

    // if (!(ctrl_data->button & PS2_BTN_UP))
    // {
    //     controller_adapter_send_cmd(adpt, PS2_BTN_UP, RT_NULL);
    // }
    // if (!(ctrl_data->button & PS2_BTN_DOWN))
    // {
    //     controller_adapter_send_cmd(adpt, PS2_BTN_DOWN, RT_NULL);
    // }
    // if (!(ctrl_data->button & PS2_BTN_LEFT))
    // {
    //     controller_adapter_send_cmd(adpt, PS2_BTN_LEFT, RT_NULL);
    // }
    // if (!(ctrl_data->button & PS2_BTN_RIGHT))
    // {
    //     controller_adapter_send_cmd(adpt, PS2_BTN_RIGHT, RT_NULL);
    // }

    return RT_EOK;
}
