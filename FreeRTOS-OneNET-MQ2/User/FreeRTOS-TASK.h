#ifndef __FREERTOS_APP_H
#define __FREERTOS_APP_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct
{
    uint8_t mq2Percent;
    uint8_t ledStatus;
}SensorData_t;




/* 全局共享数据 */
extern SensorData_t  g_sensorData;

/* 互斥量 */
extern SemaphoreHandle_t  g_sensorMutex;

/* 任务句柄 */
extern TaskHandle_t DataCollectTask_Handler;
extern TaskHandle_t OLEDDisplayTask_Handler;
extern TaskHandle_t OneNetUploadTask_Handler;


/* 对外接口 */
void App_ObjCreate(void);
void App_TaskCreate(void);

#endif