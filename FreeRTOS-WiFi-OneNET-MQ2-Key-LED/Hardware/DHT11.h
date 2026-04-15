#ifndef DHT11_H
#define DHT11_H

#include "stm32f10x.h"


// PB0
#define DHT11_IO_IN()   {GPIOB->CRL &= 0xFFFFFFF0; GPIOB->CRL |= 0x8;}
#define DHT11_IO_OUT()  {GPIOB->CRL &= 0xFFFFFFF0; GPIOB->CRL |= 0x3;}
//IO操作函数   
#define	DHT11_DQ_OUT(X)  GPIO_WriteBit(GPIOB, GPIO_Pin_0, X)
#define	DHT11_DQ_IN  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(uint8_t *temp,uint8_t *humi);//读取数据
uint8_t DHT11_Read_Byte(void);//读取一个字节
uint8_t DHT11_Read_Bit(void);//读取一位
uint8_t DHT11_Check(void);//检测DHT11
void DHT11_Rst(void);//复位DHT11   

#endif
