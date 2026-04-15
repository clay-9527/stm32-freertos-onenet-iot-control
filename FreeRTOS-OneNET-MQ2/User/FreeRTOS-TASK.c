#include "FreeRTOS-TASK.h"
#include "OLED.h"
#include "MQ2.h"
#include "UART1.h"
#include "LED.h"
#include "Key.h"
#include "onenet.h"
#include "esp8266.h"
#include "semphr.h"

/* OneNET上传变量 */
u16 MQ2 = 0;

/*-------------------- 全局共享资源 --------------------*/
SensorData_t g_sensorData = {0};
SemaphoreHandle_t g_sensorMutex = NULL;

/*-------------------- 任务句柄 --------------------*/
TaskHandle_t DataCollectTask_Handler = NULL;
TaskHandle_t OLEDDisplayTask_Handler = NULL;
TaskHandle_t OneNetUploadTask_Handler = NULL;
TaskHandle_t KeyTask_Handler = NULL;

/*-------------------- 任务优先级 --------------------*/
#define DATA_COLLECT_TASK_PRIO   3
#define OLED_DISPLAY_TASK_PRIO   2
#define ONENET_UPLOAD_TASK_PRIO  2
#define KEY_TASK_PRIO            3

/*-------------------- 任务堆栈 --------------------*/
#define DATA_COLLECT_STK_SIZE    256
#define OLED_DISPLAY_STK_SIZE    256
#define ONENET_UPLOAD_STK_SIZE   512
#define KEY_TASK_STK_SIZE        256

/*-------------------- 任务函数声明 --------------------*/
static void data_collect_task(void *pvParameters);
static void oled_display_task(void *pvParameters);
static void onenet_upload_task(void *pvParameters);
static void key_task(void *pvParameters);

/**
  * @brief  创建共享对象
  * @param  无
  * @retval 无
  */
void App_ObjCreate(void)
{
    g_sensorMutex = xSemaphoreCreateMutex();

    if(g_sensorMutex == NULL)
    {
        while(1);
    }
}

/**
  * @brief  创建应用任务
  * @param  无
  * @retval 无
  */
void App_TaskCreate(void)
{
    xTaskCreate(data_collect_task,
                "data_collect_task",
                DATA_COLLECT_STK_SIZE,
                NULL,
                DATA_COLLECT_TASK_PRIO,
                &DataCollectTask_Handler);

    xTaskCreate(oled_display_task,
                "oled_display_task",
                OLED_DISPLAY_STK_SIZE,
                NULL,
                OLED_DISPLAY_TASK_PRIO,
                &OLEDDisplayTask_Handler);

    xTaskCreate(onenet_upload_task,
                "onenet_upload_task",
                ONENET_UPLOAD_STK_SIZE,
                NULL,
                ONENET_UPLOAD_TASK_PRIO,
                &OneNetUploadTask_Handler);

    xTaskCreate(key_task,
                "key_task",
                KEY_TASK_STK_SIZE,
                NULL,
                KEY_TASK_PRIO,
                &KeyTask_Handler);
}

/**
  * @brief  MQ2采集任务
  * @param  pvParameters
  * @retval 无
  */
static void data_collect_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    uint8_t mq2Temp = 0;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        mq2Temp = (uint8_t)Get_MQ2_Percentage_value();

        if(xSemaphoreTake(g_sensorMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
            g_sensorData.mq2Percent = mq2Temp;
            xSemaphoreGive(g_sensorMutex);
        }

        UsartPrintf(USART_DEBUG, "MQ2=%d\r\n", mq2Temp);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}

/**
  * @brief  按键控灯任务
  * @param  pvParameters
  * @retval 无
  */
static void key_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    uint8_t keyNum = 0;
    uint8_t ledTemp = 0;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        keyNum = Key_GetNum();

        if(keyNum == 1)
        {
            if(xSemaphoreTake(g_sensorMutex, pdMS_TO_TICKS(20)) == pdTRUE)
            {
                ledTemp = !g_sensorData.ledStatus;
                g_sensorData.ledStatus = ledTemp;
                xSemaphoreGive(g_sensorMutex);
            }

            LED_Set(ledTemp);

            UsartPrintf(USART_DEBUG, "LED=%d\r\n", ledTemp);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    }
}

/**
  * @brief  OLED显示任务
  * @param  pvParameters
  * @retval 无
  */
static void oled_display_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    uint8_t mq2Show = 0;
    uint8_t ledShow = 0;

    xLastWakeTime = xTaskGetTickCount();

    OLED_Clear();
    OLED_ShowString(1, 1, "MQ2:");
    OLED_ShowString(2, 1, "LED:");

    while(1)
    {
        if(xSemaphoreTake(g_sensorMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
            mq2Show = g_sensorData.mq2Percent;
            ledShow = g_sensorData.ledStatus;
            xSemaphoreGive(g_sensorMutex);
        }

        OLED_ShowNum(1, 5, mq2Show, 3);
        OLED_ShowString(1, 8, "% ");

        if(ledShow)
            OLED_ShowString(2, 5, "ON ");
        else
            OLED_ShowString(2, 5, "OFF");

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(500));
    }
}

/**
  * @brief  OneNET上传任务
  * @param  pvParameters
  * @retval 无
  */
static void onenet_upload_task(void *pvParameters)
{
    TickType_t xLastWakeTime;
    uint8_t mq2Upload = 0;
    uint8_t ledUpload = 0;
    unsigned char *dataPtr = NULL;

    xLastWakeTime = xTaskGetTickCount();

    while(1)
    {
        /* 先处理平台下发数据 */
        dataPtr = ESP8266_GetIPD(0);
        if(dataPtr != NULL)
        {
            UsartPrintf(USART_DEBUG, "IPD ARRIVED\r\n");
            OneNet_RevPro(dataPtr);
            ESP8266_Clear();
        }

        if(xSemaphoreTake(g_sensorMutex, pdMS_TO_TICKS(20)) == pdTRUE)
        {
            mq2Upload = g_sensorData.mq2Percent;
            ledUpload = g_sensorData.ledStatus;
            xSemaphoreGive(g_sensorMutex);
        }

        MQ2 = mq2Upload;
        led_info.Led_Status = ledUpload;

        UsartPrintf(USART_DEBUG,
                    "UPLOAD MQ2=%d LED=%d\r\n",
                    mq2Upload, ledUpload);

        OneNet_SendData();

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    }
}