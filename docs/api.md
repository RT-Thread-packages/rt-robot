# RT-Robot API 介绍

在各个模块硬件测试通过后，会一步步将稳定的 API 添加到这里，现在大家可以参照[例程](./samples.md)里面提供的方法进行初始化和控制。



## 控制器 API (controller)

不同手柄按键指令与机器人运动指令间的转换。



## 机器人 API (robot)

包括智能小车、机械臂、飞行器。

### 智能小车 API (mobile_robot)

#### 1 车底盘 API (chassis)



##### 1.1 车轮 API (wheel)

闭环车轮的运动。



**1.1.1 电机 API (motor)**

电机控制相关。

**1.1.2 编码器 API (encoder)**

获取编码器信息。

**1.1.3 PID API (PID)**

闭环控制。



##### 1.2 动力学模型 API (kinematics)

小车速度 (x, y, w) 与各个电机转速 (rpm1, rpm2, rpm3, rpm4) 之间的相互转换。



#### 2 传感器 API (sensor)