/*********************************************************
Change From YFRobot. www.yfrobot.com
**********************************************************/

#include <rtthread.h>
#include <rtdevice.h>
#include "ps2_controller.h"
#include "ps2_controller_port.h"

extern hal_ps2_port_t hal_ps2_port;

void ps2_init(void)
{
    hal_ps2_port.init(0);
}

int ps2_scan(ps2_ctrl_data_t *pt)
{
    uint8_t temp[2] = {0x01, 0x42};
    uint8_t temp_recv[7] = {0x00};

    hal_ps2_port.send_then_recv(temp, 2, temp_recv, 7);

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

    return 0;
}

/**
 * @brief 控制手柄振动
 * @param s_motor 右侧小振动电机振动幅度, 0x00 关, 其它开？
 * @param l_motor 左侧大振动电机振动幅度, 0x40~0xFF 电机开, 值越大振动越大
 * @note 需先调用 ps2_enter_config
 */
void ps2_vibrating(uint8_t s_motor, uint8_t l_motor)
{
    uint8_t temp[9];

    temp[0] = 0x01;
    temp[1] = 0x42;
    temp[3] = s_motor;
    temp[4] = l_motor;
    hal_ps2_port.send(temp, sizeof(temp));
}

void ps2_enter_config(void)
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
    hal_ps2_port.send(temp, sizeof(temp));
}

void ps2_open_vibration_mode(void)
{
    uint8_t temp[5] = {0};

    temp[0] = 0x01;
    temp[1] = 0x4D;
    temp[2] = 0x00;
    temp[3] = 0x00;
    temp[4] = 0x01;
    hal_ps2_port.send(temp, sizeof(temp));
}

void ps2_exit_config(void)
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
    hal_ps2_port.send(temp, sizeof(temp));
}
