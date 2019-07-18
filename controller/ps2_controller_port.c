#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "ps2_controller_port.h"

#define HAL_CS_PIN      GET_PIN(B, 12)
#define HAL_CLK_PIN     GET_PIN(B, 13)
#define HAL_DO_PIN      GET_PIN(B, 15)
#define HAL_DI_PIN      GET_PIN(B, 14)

#define HAL_CS_H()      rt_pin_write(HAL_CS_PIN, PIN_HIGH)
#define HAL_CS_L()      rt_pin_write(HAL_CS_PIN, PIN_LOW)
#define HAL_CLK_H()     rt_pin_write(HAL_CLK_PIN, PIN_HIGH)
#define HAL_CLK_L()     rt_pin_write(HAL_CLK_PIN, PIN_LOW)
#define HAL_DO_H()      rt_pin_write(HAL_DO_PIN, PIN_HIGH)
#define HAL_DO_L()      rt_pin_write(HAL_DO_PIN, PIN_LOW)
#define HAL_GET_DI()    rt_pin_read(HAL_DI_PIN)

#define KEEP_TIME() _delay_us(5);

static void _delay_us(uint16_t us)
{
    // TODO

    for (int i = 0; i < us; i++)
    {
        // maybe the cyclic value needs to be modified
        for (int j = 0; j < 0x1F;)
            j++;
    }
}

static void _send_data(uint8_t data)
{
    for (uint16_t i = 0x01; i < 0x0100; i <<= 1)
    {
        if (i & data)
            HAL_DO_H(); 
        else
            HAL_DO_L();

        HAL_CLK_H();
        KEEP_TIME();
        HAL_CLK_L();
        KEEP_TIME();
        HAL_CLK_H();
    }
}

static uint8_t _recv_data(void)
{
    uint8_t temp = 0;

    for (uint16_t i = 0x01; i < 0x100; i <<= 1)
    {
        HAL_CLK_H();
        KEEP_TIME();
        HAL_CLK_L();
        KEEP_TIME();
        if (HAL_GET_DI())
            temp = i | temp;
        HAL_CLK_H();
        
    }

    return temp;
}

static void send(const uint8_t *pb, uint8_t len)
{
    HAL_CS_L();
    _delay_us(16);
    for (uint8_t i = 0; i < len; i++)
    {
        _send_data(pb[i]);
    }
    HAL_CS_H();
    _delay_us(16);
}

static void send_then_recv(const uint8_t *pb_send, uint8_t len_send, uint8_t *pb_recv, uint8_t len_recv)
{
    HAL_CS_L();
    for (uint8_t i = 0; i < len_send; i++)
    {
        _send_data(pb_send[i]);
    }
    for (uint8_t i = 0; i < len_recv; i++)
    {
        pb_recv[i] = _recv_data();
    }
    HAL_CS_H();
}

static void transfer(const uint8_t *pb_send, uint8_t *pb_recv, uint8_t len)
{
    // TODO
    ;
}

static int init(void *vp)
{
    rt_pin_mode(HAL_CS_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_CLK_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_DO_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(HAL_DI_PIN, PIN_MODE_INPUT);

    HAL_CS_H();
    HAL_CLK_H();

    return 0;
}

hal_ps2_port_t hal_ps2_port = {
    .init = init,
    .send = send,
    .send_then_recv = send_then_recv,
    .transfer = transfer,
};
