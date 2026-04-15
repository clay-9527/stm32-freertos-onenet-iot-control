/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-29     LCKFB-LP    first version
 */
#ifndef _MQ2_H_
#define _MQ2_H_

#include "stm32f10x.h"

#define RCC_MQ2_GPIO    RCC_APB2Periph_GPIOA

#define RCU_MQ2_ADC     RCC_APB2Periph_ADC1

#define PORT_ADC        ADC1
#define CHANNEL_ADC     ADC_Channel_4

#define PORT_MQ2        GPIOA

#define GPIO_MQ2_AO     GPIO_Pin_4
#define GPIO_MQ2_DO     GPIO_Pin_1

 //采样次数
#define SAMPLES         30

/************************

//之前的单路采集
void ADC_Init(void);
unsigned int Get_ADC_Value(void);

**************************/
void Adc_Init(void);
unsigned int Get_Adc_Value(void);
unsigned int Get_MQ2_Percentage_value(void);

#endif



