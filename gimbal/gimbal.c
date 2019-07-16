
/*
 reference to Robomaster 2019 
*/
#include "gimbal.h"

#define     DBG_SECTION_NAME    "gimbal"
#define     DBG_LEVEL           DBG_LOG
#include <rtdbg.h>


//gimbal_t gimbal_find (const char * name)
//{
//    struct object * object;
//    object = object_find (name, Object_Class_Gimbal);

//    return (gimbal_t)
//    object;
//}


rt_err_t gimbal_pitch_enable (struct gimbal * gimbal)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    //to fill in controller_enable
    //controller_enable(&(gimbal->ctrl[PITCH_MOTOR_INDEX]));
    return RT_EOK;
}


rt_err_t gimbal_pitch_disable (struct gimbal * gimbal)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    //to fill in controller_disable
    //controller_disable(&(gimbal->ctrl[PITCH_MOTOR_INDEX]));
    return RT_EOK;
}


rt_err_t gimbal_yaw_enable (struct gimbal * gimbal)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    //controller_enable(&(gimbal->ctrl[YAW_MOTOR_INDEX]));
    return RT_EOK;
}


rt_err_t gimbal_yaw_disable (struct gimbal * gimbal)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    //controller_disable(&(gimbal->ctrl[YAW_MOTOR_INDEX]));
    return RT_EOK;
}


rt_err_t gimbal_disable (struct gimbal * gimbal)
{

    if (gimbal == NULL)
        return - RT_EINVAL;

    rt_err_t err_y = 0, err_p = 0;

    err_y = gimbal_yaw_disable (gimbal);
    err_p = gimbal_pitch_disable (gimbal);

    if ((err_y == RT_EOK) && (err_p == RT_EOK))
    {
        return RT_EOK;
    }
    else 
    {
        return - RT_ERRTR;
    }
}


rt_err_t gimbal_enable (struct gimbal * gimbal)
{

    if (gimbal == NULL)
        return - RT_EINVAL;

    rt_err_t err_y = 0, err_p = 0;

    err_y = gimbal_yaw_enable (gimbal);
    err_p = gimbal_pitch_enable (gimbal);

    if ((err_y == RT_EOK) && (err_p == RT_EOK))
    {
        return RT_EOK;
    }
    else 
    {
        return - RT_ERRTR;
    }
}


rt_err_t gimbal_set_pitch_angle (struct gimbal * gimbal, float pitch)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    if (gimbal->mode.bit.pitch_mode == GYRO_MODE)
    {

    }
    else 
    {

    }

    return RT_EOK;
}


rt_err_t gimbal_set_yaw_angle (struct gimbal * gimbal, float yaw, uint8_t mode) //yaw mode：
{ //1.clockwise 2.anticlockwise

    if (gimbal == NULL)
        return - RT_EINVAL;

    if (gimbal->mode.bit.yaw_mode == GYRO_MODE)
    {

    }
    else 
    {

    }

    return RT_EOK;
}


rt_err_t gimbal_rate_update (struct gimbal * gimbal, float yaw_rate, float pitch_rate)
{
    if (gimbal == NULL)
        return - RT_EINVAL;

    gimbal->sensor.rate.yaw_rate = yaw_rate;
    gimbal->sensor.rate.pitch_rate = pitch_rate;

    return RT_EOK;
}


