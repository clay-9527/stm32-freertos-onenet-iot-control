#include "stm32f10x.h"                  // Device header
#include "MQ2.h"
#include "Delay.h"

/******************************************************************
 * 函 数 名 称：Adc_Init
 * 函 数 说 明：初始化ADC功能
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LC
 * 备       注：无
******************************************************************/
void Adc_Init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure;
        ADC_InitTypeDef ADC_InitStruct;

        RCC_APB2PeriphClockCmd(RCU_MQ2_ADC|RCC_MQ2_GPIO, ENABLE);

        GPIO_InitStructure.GPIO_Pin = GPIO_MQ2_AO;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
        GPIO_Init(PORT_MQ2, &GPIO_InitStructure);

        ADC_DeInit(PORT_ADC);//ADC复位

        // ADC 模式配置
        ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;// 只使用一个ADC，独立模式
        ADC_InitStruct.ADC_ScanConvMode = DISABLE ;// 禁止扫描模式，单通道不需要扫描
        ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;// 连续转换模式
        ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// 软件触发转换
        ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;// 数据右对齐
        ADC_InitStruct.ADC_NbrOfChannel = 1;// 转换通道1个

        // 初始化ADC
        ADC_Init(PORT_ADC, &ADC_InitStruct);

        // 配置ADC时钟PCLK2的6分频，即12MHz
        RCC_ADCCLKConfig(RCC_PCLK2_Div6);

        // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为55.5个时钟周期
        // 通道为5
        ADC_RegularChannelConfig(PORT_ADC, CHANNEL_ADC, 1, ADC_SampleTime_55Cycles5);

        //ADC使能
        ADC_Cmd(PORT_ADC, ENABLE);

        // 初始化ADC 校准寄存器
        ADC_ResetCalibration(PORT_ADC);
        // 等待校准寄存器初始化完成
        while (ADC_GetResetCalibrationStatus(PORT_ADC));

        // ADC开始校准
        ADC_StartCalibration(PORT_ADC);
        // 等待校准完成
        while (ADC_GetCalibrationStatus(PORT_ADC));

        // 使用软件触发ADC转换
        ADC_SoftwareStartConvCmd(PORT_ADC, ENABLE);
}

/******************************************************************
 * 函 数 名 称：Get_Adc_Value
 * 函 数 说 明：
 * 函 数 形 参：
 * 函 数 返 回：对应扫描的ADC值
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned int Get_Adc_Value(void)
{
        unsigned char i = 0;
        unsigned int AdcValue = 0;

        /* 因为采集 SAMPLES 次，故循环 SAMPLES 次 */
        for(i=0; i< SAMPLES; i++)
        {
                /*    累加    */
                AdcValue += ADC_GetConversionValue(PORT_ADC);
        }
        /* 求平均值 */
        AdcValue = AdcValue / SAMPLES;

        return AdcValue;
}

/******************************************************************
 * 函 数 名 称：Get_MQ2_Percentage_value
 * 函 数 说 明：读取MQ2值，并且返回百分比
 * 函 数 形 参：无
 * 函 数 返 回：返回百分比
 * 作       者：LC
 * 备       注：无
******************************************************************/
unsigned int Get_MQ2_Percentage_value(void)
{
    int adc_max = 4095;
    int adc_new = 0;
    int Percentage_value = 0;

    adc_new = Get_Adc_Value();

    Percentage_value = ((float)adc_new/(float)adc_max) * 100.f;
    return Percentage_value;
}



