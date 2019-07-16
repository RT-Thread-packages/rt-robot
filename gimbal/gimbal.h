
/*
 reference to Robomaster 2019 
*/
#ifndef __GIMBAL_H__
#define __GIMBAL_H__

//#include "ro_object.h"
#include "string.h"
#include "ro_errco.h"

#include "rtthread.h"

#define     YAW_MOTOR_INDEX     0
#define     PITCH_MOTOR_INDEX   1

#define     ENCODER_MODE        (0u)
#define     GYRO_MODE           (1u)

#define     YAW_CLOCKWISE       (1u)
#define     YAW_ANTICLOCKWISE   (2u)

typedef struct gimbal * gimbal_t;


struct gimbal_param
{
    rt_int16_t      pitch_ecd_center;
    rt_int16_t      yaw_ecd_center;
};


struct gimbal_p_y
{
/* unit: degree */
    float           yaw;
    float           pitch;
};

struct gimbal_rate
{
/* unit: degree/s */
    float           yaw_rate;
    float           pitch_rate;
};

struct gimbal_sensor
{
    struct gimbal_p_y gyro_angle;
    struct gimbal_rate rate;
};

struct gimbal
{
    //struct object parent;
    struct gimbal_param param; //storage ecd/Gyro data when gimbal in center position
    union 
    {                                                   //for gimbal added Gyro to detect whether 
      rt_uint8_t      state;                              //gimbla turn over the angle limitted
      struct 
     {
        rt_uint8_t      yaw_mode        : 1;
        rt_uint8_t      pitch_mode      : 1;
     } bit;
    } mode;

    struct gimbal_sensor sensor; //storage Gyro data for gimbal added Gyro
    struct gimbal_p_y ecd_angle; //storage encoder data
    struct gimbal_p_y gyro_target_angle;
    struct gimbal_p_y ecd_target_angle;
//  struct motor_device motor[2];
//  struct cascade cascade[2];    //cascade pid para for yaw/pit
//  struct cascade_feedback cascade_fdb[2];   //feedback data
//  struct controller ctrl[2];                //yaw/pit controller algorithm
};


gimbal_t gimbal_find (const char * name);

rt_err_t gimbal_enable (struct gimbal * gimbal);
rt_err_t gimbal_disable (struct gimbal * gimbal);

rt_err_t gimbal_pitch_enable (struct gimbal * gimbal);
rt_err_t gimbal_pitch_disable (struct gimbal * gimbal);
rt_err_t gimbal_yaw_enable (struct gimbal * gimbal);
rt_err_t gimbal_yaw_disable (struct gimbal * gimbal);

rt_err_t gimbal_set_pitch_angle (struct gimbal * gimbal, float pitch);
rt_err_t gimbal_set_yaw_angle (struct gimbal * gimbal, float yaw, uint8_t mode);        //yaw mode  
rt_err_t gimbal_rate_update (struct gimbal * gimbal, float yaw_rate, float pitch_rate); //1.clockwise 2.anticlockwise


#endif //  __GIMBAL_H__

