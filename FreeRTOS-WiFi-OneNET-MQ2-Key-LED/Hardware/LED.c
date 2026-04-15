#include "stm32f10x.h"                  // Device header
#include "LED.h" 

LED_INFO led_info = {0};

void LED_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);						
	
	LED_Set(LED_OFF);				
	
}

void LED_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, status == LED_ON ? Bit_SET : Bit_RESET);	
	led_info.Led_Status = status;

}
