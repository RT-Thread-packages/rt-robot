#ifndef __PS2_H__
#define __PS2_H__

#include <stdint.h>

#define PS2_BTN_SELECT       0
#define PS2_BTN_L3           1
#define PS2_BTN_R3           2
#define PS2_BTN_START        3
#define PS2_BTN_UP           4
#define PS2_BTN_RIGHT        5
#define PS2_BTN_DOWN         6
#define PS2_BTN_LEFT         7
#define PS2_BTN_L2           8
#define PS2_BTN_R2           9
#define PS2_BTN_L1           10
#define PS2_BTN_R1           11
#define PS2_BTN_TRIANGLE     12
#define PS2_BTN_CICLE        13
#define PS2_BTN_FORK         14
#define PS2_BTN_SQUARE       15
#define PS2_STICK_LX         16
#define PS2_STICK_LY         17
#define PS2_STICK_RX         18
#define PS2_STICK_RY         19

typedef struct ps2_ctrl_data ps2_ctrl_data_t;

struct ps2_ctrl_data
{
    uint16_t button;        // 16
    uint8_t  left_stick_x;
    uint8_t  left_stick_y;
    uint8_t  right_stick_x;
    uint8_t  right_stick_y;
};

typedef struct ps2_controller ps2_controller_t;

struct ps2_controller
{
    rt_base_t pin_cs;
    rt_base_t pin_clk;
    rt_base_t pin_do;
    rt_base_t pin_di;
    ps2_ctrl_data_t data;
};

#define HAL_CS_PIN      51      // PB12:  SPI2_CS
#define HAL_CLK_PIN     52      // PB13:  SPI2_SCK
#define HAL_DO_PIN      54      // PB15:  SPI2_MOSI
#define HAL_DI_PIN      53      // PB14:  SPI2_MISO

#define HAL_CS_H()      rt_pin_write(HAL_CS_PIN, PIN_HIGH)
#define HAL_CS_L()      rt_pin_write(HAL_CS_PIN, PIN_LOW)
#define HAL_CLK_H()     rt_pin_write(HAL_CLK_PIN, PIN_HIGH)
#define HAL_CLK_L()     rt_pin_write(HAL_CLK_PIN, PIN_LOW)
#define HAL_DO_H()      rt_pin_write(HAL_DO_PIN, PIN_HIGH)
#define HAL_DO_L()      rt_pin_write(HAL_DO_PIN, PIN_LOW)
#define HAL_GET_DI()    rt_pin_read(HAL_DI_PIN)

#define KEEP_TIME()     ps2_controller_delay(5);

static int      ps2_controller_init(ps2_controller_t ps2);
static void     ps2_controller_delay(uint16_t us);
static uint8_t  ps2_controller_recv_data(ps2_controller_t ps2);
static void     ps2_controller_send_byte(ps2_controller_t ps2, uint8_t data);
static void     ps2_controller_send_bytes(ps2_controller_t ps2, const uint8_t *pb, uint8_t len);
static void     ps2_controller_send_recv(ps2_controller_t ps2, const uint8_t *pb_send, uint8_t len_send, uint8_t *pb_recv, uint8_t len_recv);
static void     ps2_controller_transfer(ps2_controller_t ps2, const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len);
static void     ps2_controller_enter_config(ps2_controller_t ps2);
static void     ps2_controller_exit_config(ps2_controller_t ps2);
static void     ps2_controller_open_vibration_mode(ps2_controller_t ps2);
static int      ps2_controller_scan(ps2_controller_t ps2, ps2_ctrl_data_t *pt);
static void     ps2_controller_vibrate(ps2_controller_t ps2, uint8_t s_motor, uint8_t l_motor);
static rt_err_t ps2_controller_parse_cmd(controller_t contrl, ps2_ctrl_data_t *ctrl_data);

controller_t    ps2_controller_create(rt_base_t pin_cs, rt_base_t pin_clk, rt_base_t pin_do, rt_base_t pin_di);
void            ps2_controller_destroy(controller_t ps2);

#endif
