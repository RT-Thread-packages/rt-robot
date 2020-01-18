# RT-Robot API 介绍

## 协议 API (protocol)



## 机器人 API (robot)

包括智能小车、机械臂、飞行器。

### 智能小车 API (mobile_robot)

#### 1 车底盘 API (chassis)

```c
chassis_t chassis_create(wheel_t* c_wheel, kinematics_t c_kinematics);
```

创建车底盘

|**参数**     | **描述**                 |
| :------- | :------------------- |
| c_wheel  | 车轮对象数组 |
| c_kinematics | 动力学模型对象 |
| **返回** | **--**             |
|   |  |

```c
rt_err_t chassis_set_velocity(chassis_t chas, struct velocity target_velocity);
```

设置小车底盘速度

|**参数**     | **描述**                 |
| :------- | :------------------- |
| chas  | 车底盘对象 |
| target_velocity | 预设速度(x,y,w)。x、y速度单位为m/s，w单位为rad/s|
| **返回** | **--**             |
|   |  |

```c
rt_err_t chassis_set_rpm(chassis_t chas, rt_int16_t target_rpm[]);
```

设置小车底盘车轮速度（单位：转/分）

|**参数**     | **描述**                 |
| :------- | :------------------- |
| chas  | 车底盘 |
| target_rpm | 预设速度(转每分) |
| **返回** | **--**             |
|   |  |

##### 1.1 车轮 API (wheel)

```c
wheel_t wheel_create(motor_t w_motor, encoder_t w_encoder, controller_t w_controller, float radius, rt_uint16_t gear_ratio);
```

创建车轮

|**参数**     | **描述**                 |
| :------- | :------------------- |
| w_motor  | 电机对象 |
| w_encoder | 编码器对象 |
| w_controller | 控制器对象 |
| radius | 车轮直径 |
| gear_ratio | 电机减速比 |
| **返回** | **--**             |
|   |  |

```c
rt_err_t wheel_set_speed(wheel_t whl, double speed);
```

设定车轮速度

|**参数**     | **描述**                 |
| :------- | :------------------- |
| whl  | 车轮对象 |
| speed | 速度（单位：m/s） |
| **返回** | **--**             |
|   |  |

```c
rt_err_t wheel_set_rpm(wheel_t whl, rt_int16_t rpm);
```

设定车轮速度

|**参数**     | **描述**                 |
| :------- | :------------------- |
| whl  | 车轮对象 |
| rpm | 速度（单位：rpm |
| **返回** | **--**             |
|   |  |

**1.1.1 电机 API (motor)**

**直流电机**

```c
dual_pwm_motor_t dual_pwm_motor_create(char *pwm1, int pwm1_channel, char *pwm2, int pwm2_channel);
```

创建双 PWM 驱动方式电机

|**参数**     | **描述**                 |
| :------- | :------------------- |
| pwm1  | pwm设备名 |
| pwm1_channel | pwm通道 |
| pwm2 | pwm设备名 |
| pwm2_channel | pwm通道 |
| **返回** | **--**             |
|   |  |

```c
single_pwm_motor_t  single_pwm_motor_create(char *pwm, int channel, rt_base_t pin1, rt_base_t pin2);
```

创建单 PWM 驱动方式电机

|**参数**     | **描述**                 |
| :------- | :------------------- |
| pwm  | pwm设备名 |
| channel | pwm通道 |
| pin1 | 控制引脚1 |
| pin2 | 控制引脚2 |
| **返回** | **--**             |
|   |  |

**舵机**

```c
servo_t servo_create(const char * pwm, int channel, float angle, rt_uint32_t pluse_min, rt_uint32_t pluse_max);
```

创建舵机

|**参数**     | **描述**                 |
| :------- | :------------------- |
| pwm  | pwm 设备名 |
| channel | pwm 通道 |
|angle|角度范围|
| pluse_min | 最小脉宽 （单位：ns）；传入 RT_NULL 则设为默认值 |
| pluse_max | 最大脉宽 （单位：ns）；传入 RT_NULL 则设为默认值|
| **返回** | **--**             |
|   |  |

```c
rt_err_t servo_set_angle(servo_t servo, float angle);
```

设定舵机转动角度

|**参数**     | **描述**                 |
| :------- | :------------------- |
| servo  | pwm 设备名 |
| angle | 转动角度 （从零至最大值，最大值取决于 create 时传入的 angle 参数）|
| **返回** | **--**             |
|   |  |


**1.1.2 编码器 API (encoder)**

获取编码器信息。

```c
ab_phase_encoder_t ab_phase_encoder_create(rt_base_t pin_a, rt_base_t pin_b, rt_uint16_t pulse_revol);
```

创建AB相编码器

|**参数**     | **描述**                 |
| :------- | :------------------- |
| pin_a  | A相引脚 |
| pin_b | B相引脚 |
| pulse_revol | 车轮转一圈输出的脉冲数 |
| **返回** | **--**             |
|   |  |

```c
single_phase_encoder_t single_phase_encoder_create(rt_base_t pin, rt_uint16_t pulse_revol);
```

创建单相编码器

|**参数**     | **描述**                 |
| :------- | :------------------- |
| pin  | 引脚 |
| pulse_revol | 车轮转一圈输出的脉冲数 |
| **返回** | **--**             |
|   |  |

**1.1.3 控制器 API (controller)**

```c
inc_pid_controller_t    inc_pid_controller_create(float kp, float ki, float kd, rt_uint16_t sample_time);
```

创建增量式 PID 控制器

|**参数**     | **描述**                 |
| :------- | :------------------- |
| kp  | 参数Kp值 |
| ki | 参数Ki值 |
| kd | 参数Kd值 |
| sample_time | 计算周期 |
| **返回** | **--**             |
|   |  |

```c
pos_pid_controller_t    pos_pid_controller_create(float kp, float ki, float kd, rt_uint16_t sample_time);
```

创建位置式 PID 控制器

|**参数**     | **描述**                 |
| :------- | :------------------- |
| kp  | 参数Kp值 |
| ki | 参数Ki值 |
| kd | 参数Kd值 |
| sample_time | 计算周期 |
| **返回** | **--**             |
|   |  |

##### 1.2 动力学模型 API (kinematics)

```c
kinematics_t    kinematics_create(enum base k_base, float length_x, float length_y, float wheel_radius);
```

创建动力学模型

|**参数**     | **描述**                 |
| :------- | :------------------- |
| k_base  | 类型 |
| length_x | X轴两轮间距 |
| length_y | Y轴两轮间距 |
| wheel_radius | 车轮半径 |
| **返回** | **--**             |
|   |  |

_小车速度 (x, y, w) 与各个电机转速 (rpm1, rpm2, rpm3, rpm4) 之间的相互转换。_

```c
rt_int16_t*     kinematics_get_rpm(struct kinematics kin, struct velocity target_vel);
```

```c
struct velocity kinematics_get_velocity(struct kinematics kin, struct rpm current_rpm);
```
