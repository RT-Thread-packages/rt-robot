# RT-Robot

## 1、介绍

RT-Robot 是 RT-Thread 的机器人框架，希望能够支持智能小车、机械臂、无人机等各种不同类型的机器人。

当前以智能车为主要目标，希望支持两轮差分驱动、四轮差分驱动、麦克纳姆轮驱动、经典 Ackerman (两轮差分，一方向连杆) 的小车底盘。

当前功能特点：
- 支持两轮差分驱动、四轮差分驱动、麦克纳姆轮驱动的小车底盘

- 支持增量、位置式 PID
- 支持单相、AB 相编码器
- 支持双 PWM、单 PWM 的直流电机驱动，支持驱动舵机
- 支持 PS2 遥控器
- 支持 ANO_TC 匿名科创地面站

### 1.1 许可证

rt-robot 软件包遵循 Apache-2.0 许可，详见 LICENSE 文件。

### 1.2 依赖
- RT-Thread 3.1.x +

## 2、获取软件包

使用 `rt-robot` 软件包需要在 BSP 目录下使用 menuconfig 命令打开 Env 配置界面，在 `RT-Thread online packages → system packages` 中选择 RT-Robot 软件包，具体路径如下：

```shell
RT-Thread online packages
    system packages  --->
         [*] RT-Robot: RT-Thread robots development platform  --->
            Version (latest) --->
```

配置完成后让 RT-Thread 的包管理器自动更新，或者使用 pkgs --update 命令更新包到 BSP 中。

## 3、使用 rt-robot

- 软件包详细介绍，请参考 [软件包介绍](docs/introduction.md)

- 如何从零开始使用，请参考 [用户指南](docs/user-guide.md)
- 框架设计说明，请参考 [设计手册](docs/design.md)
- 完整的 API 文档，请参考 [API 手册](docs/api.md)
- 详细的示例介绍，请参考 [示例文档](docs/samples.md)

## 4、注意事项

- 当前框架正在逐步完善，欢迎大家来提 PR

## 5、联系方式 & 感谢

| 维护     | 主页       | 邮箱  | 
| -------- | ---------- | --- | 
| Wu Han | http://wuhanstudio.cc   | wuhanstudio@hust.edu.cn |
| sogwms | https://github.com/sogwms | sogwyms@gmail.com |
