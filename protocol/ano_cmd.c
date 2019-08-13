#include "ano.h"
#include "command.h"

static void request_pid_param_info(void)
{
    command_handle(COMMAND_REQUEST_PID, RT_NULL, 0, RT_NULL);
}
static void reset_param(void)
{

}
static void set_pid_group1(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{
    struct cmd_dt_pid pid;

    pid.id = 1;
    pid.kp = k1_p;
    pid.ki = k1_i;
    pid.kd = k1_d;
    command_handle(COMMAND_SET_PID, &pid, sizeof(struct cmd_dt_pid), RT_NULL);
    pid.id = 2;
    pid.kp = k2_p;
    pid.ki = k2_i;
    pid.kd = k2_d;
    command_handle(COMMAND_SET_PID, &pid, sizeof(struct cmd_dt_pid), RT_NULL);
    pid.id = 3;
    pid.kp = k3_p;
    pid.ki = k3_i;
    pid.kd = k3_d;
    command_handle(COMMAND_SET_PID, &pid, sizeof(struct cmd_dt_pid), RT_NULL);

    
}
static void set_pid_group2(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{

}
static void set_pid_group3(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{

}
static void set_pid_group4(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{

}
static void set_pid_group5(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{

}
static void set_pid_group6(float k1_p, float k1_i, float k1_d, float k2_p, float k2_i, float k2_d, float k3_p, float k3_i, float k3_d)
{

}

static struct ano_callback cb = {
    .request_pid_param_info = request_pid_param_info,
    .reset_param = reset_param,
    .set_pid_group1 = set_pid_group1,
    .set_pid_group2 = set_pid_group2,
    .set_pid_group3 = set_pid_group3,
    .set_pid_group4 = set_pid_group4,
    .set_pid_group5 = set_pid_group5,
    .set_pid_group6 = set_pid_group6,
};

static void ano_command_register(void)
{
    // set ano callback
    ano_set_callback(&cb);
}

INIT_DEVICE_EXPORT(ano_command_register);
