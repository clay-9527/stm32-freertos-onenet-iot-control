# stm32-freertos-onenet-iot-control
基于 STM32、FreeRTOS、ESP8266-01S 与 OneNET 云平台实现 MQ2 气体监测及灯光远程控制，并搭配 HBuilder X 开发的移动端 APP。
本项目是一个基于 STM32 + FreeRTOS + ESP8266-01S + OneNET 云平台 的智能家居物联网系统。
系统主要实现以下功能：
ESP8266-01S WiFi 联网
OneNET 云端 MQTT 数据上传与指令订阅
MQ2 气体浓度实时采集
手机 APP 远程开关灯
HBuilder X（uni-app）移动端控制界面
FreeRTOS 多任务实时调度

该项目适用于：
FreeRTOS 学习实践
IoT 云平台开发入门
智能家居/智能硬件比赛项目
毕设/课程设计

硬件架构
主控芯片： STM32F103C8T6
实时系统： FreeRTOS
WiFi 模块： ESP8266-01S
云平台： OneNET
传感器： MQ2 烟雾传感器
执行器： LED
移动端： HBuilder X（uni-app）

核心功能模块
1）FreeRTOS 系统移植
STM32 平台 FreeRTOS 内核移植
多任务优先级调度
延时与时间片管理

2）ESP8266-01S + OneNET 云平台
AT 指令联网
MQTT 长连接
设备属性上传
云端控制指令下发

3）MQ2 数据采集
ADC 周期采样
数据滤波
阈值报警预留
云端实时数据显示

4）远程灯光控制
APP 按钮 → 云平台 → STM32 执行
LED / 继电器开关控制
状态实时回传

5）HBuilder X 手机 APP
设备状态显示
MQ2 浓度数据显示
灯光控制按钮
云端状态同步
