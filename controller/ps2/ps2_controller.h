#ifndef __PS2_H__
#define __PS2_H__

#include <stdint.h>

#define PS2_BUTTON_SELECT       (1 << 0)
#define PS2_BUTTON_L3           (1 << 1)
#define PS2_BUTTON_R3           (1 << 2)
#define PS2_BUTTON_START        (1 << 3)
#define PS2_BUTTON_UP           (1 << 4)
#define PS2_BUTTON_RIGHT        (1 << 5)
#define PS2_BUTTON_DOWN         (1 << 6)
#define PS2_BUTTON_LEFT         (1 << 7)
#define PS2_BUTTON_L2           (1 << 8)
#define PS2_BUTTON_R2           (1 << 9)
#define PS2_BUTTON_L1           (1 << 10)
#define PS2_BUTTON_R1           (1 << 11)
#define PS2_BUTTON_TRIANGLE     (1 << 12)
#define PS2_BUTTON_CICLE        (1 << 13)
#define PS2_BUTTON_FORK         (1 << 14)
#define PS2_BUTTON_SQUARE       (1 << 15)

typedef struct ps2_ctrl_data ps2_ctrl_data_t;

struct ps2_ctrl_data
{
    uint16_t button;        // 16
    uint8_t left_stick_x;
    uint8_t left_stick_y;
    uint8_t right_stick_x;
    uint8_t right_stick_y;
};

void ps2_init(void);
int ps2_scan(ps2_ctrl_data_t *pt);
void ps2_vibrating(uint8_t s_motor, uint8_t l_motor);
void ps2_enter_config(void);
void ps2_open_vibration_mode(void);
void ps2_exit_config(void);

#endif
