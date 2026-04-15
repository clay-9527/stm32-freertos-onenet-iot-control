#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "OLED.h"
#include "MQ2.h"
#include "UART1.h"
#include "LED.h"
#include "Key.h"
#include "onenet.h"
#include "esp8266.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Delay.h"

#include "FreeRTOS-TASK.h"

/* OneNET服务器连接信息 */
#define ESP8266_ONENET_INFO  "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

/* 外部函数声明 */
static void Hardware_Init(void);
static void OneNET_NetworkInit(void);

/* 开始任务句柄 */
TaskHandle_t StartTask_Handler = NULL;

/* 开始任务配置 */
#define START_TASK_PRIO      1
#define START_STK_SIZE       512

/* 开始任务函数声明 */
static void start_task(void *pvParameters);

int main(void)
{
  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 			 /* 中断优先级分组 */
   
    Hardware_Init(); 														/* 硬件初始化 */
  
    App_ObjCreate(); 			 										/* 创建共享资源 */
		
    OneNET_NetworkInit();									/* 先联网 */
  
    xTaskCreate(start_task, 								 /* 创建开始任务 */
                "start_task",
                START_STK_SIZE,
                NULL,
                START_TASK_PRIO,
                &StartTask_Handler);
   
    vTaskStartScheduler(); 							/* 启动调度器 */

    /* 正常不会到这里 */
    while(1)
    {
    }
}

/**
  * @brief  硬件初始化
  * @param  无
  * @retval 无
  */
static void Hardware_Init(void)
{
    Usart1_Init(115200);      /* 串口1：调试输出 */
    Usart2_Init(115200);      /* 串口2：ESP8266通信 */
    OLED_Init();
    LED_Init();
    Key_Init();
    Adc_Init();
		DHT11_Init();
    OLED_Clear();
    OLED_ShowString(1, 1, "System Init");
    UsartPrintf(USART_DEBUG, "Hardware init OK\r\n");
    ESP8266_Init();

}

/**
  * @brief  OneNET网络初始化
  * @param  无
  * @retval 无
  */
static void OneNET_NetworkInit(void)
{
    OLED_Clear();
    OLED_ShowString(1, 1, "Net Connecting");
    UsartPrintf(USART_DEBUG, "Connect MQTT Server...\r\n");
    while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
    {
        Delay_ms(500);
    }
    UsartPrintf(USART_DEBUG, "Connect MQTT Successfully\r\n");
    while(OneNet_DevLink())
    {
        Delay_ms(500);
    }
    OneNET_Subscribe();
    OLED_Clear();
    OLED_ShowString(1, 1, "OneNET Ready");
    UsartPrintf(USART_DEBUG, "OneNET Ready\r\n");
}

/**
  * @brief  开始任务
  * @param  pvParameters: 任务参数
  * @retval 无
  */
static void start_task(void *pvParameters)
{
		taskENTER_CRITICAL();   /* 进入临界区，防止创建任务过程被打断 */
    App_TaskCreate();  /* 创建应用任务 */
    vTaskDelete(StartTask_Handler);   /* 删除开始任务 */
    taskEXIT_CRITICAL();    /* 退出临界区 */
}