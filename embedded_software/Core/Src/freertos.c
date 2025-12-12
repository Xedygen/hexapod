/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ibus.h"
#include "mpu6050.h"
#include "pca9685.h"
#include <stdio.h>
#include "fatfs.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define DEBUG_UART &huart2
#define INCLUDE_vTaskDelete 1

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

char logbuf[256];
IBUS_Handle_t ibus;
IBus_Data_t data;
MPU6050_Data_t mpu;
PCA9685_Handle_t pca;
PCA9685_Handle_t pca1;

FATFS fs;
FRESULT fr;
FIL file;
UINT bytes;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for IBusTask */
osThreadId_t IBusTaskHandle;
const osThreadAttr_t IBusTask_attributes = {
  .name = "IBusTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ServoTask */
osThreadId_t ServoTaskHandle;
const osThreadAttr_t ServoTask_attributes = {
  .name = "ServoTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for IMUTask */
osThreadId_t IMUTaskHandle;
const osThreadAttr_t IMUTask_attributes = {
  .name = "IMUTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LEDBlinkTask */
osThreadId_t LEDBlinkTaskHandle;
const osThreadAttr_t LEDBlinkTask_attributes = {
  .name = "LEDBlinkTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SDLogTask */
osThreadId_t SDLogTaskHandle;
const osThreadAttr_t SDLogTask_attributes = {
  .name = "SDLogTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for debugPrintTask */
osThreadId_t debugPrintTaskHandle;
const osThreadAttr_t debugPrintTask_attributes = {
  .name = "debugPrintTask",
  .stack_size = 1024 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {
  .name = "uartMutex"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartIBusTask(void *argument);
void StartServoTask(void *argument);
void StartIMUTask(void *argument);
void StartLEDBlinkTask(void *argument);
void StartSDLogTask(void *argument);
void StartDebugPrintTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of uartMutex */
  uartMutexHandle = osMutexNew(&uartMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of IBusTask */
  IBusTaskHandle = osThreadNew(StartIBusTask, NULL, &IBusTask_attributes);

  /* creation of ServoTask */
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_attributes);

  /* creation of IMUTask */
  IMUTaskHandle = osThreadNew(StartIMUTask, NULL, &IMUTask_attributes);

  /* creation of LEDBlinkTask */
  LEDBlinkTaskHandle = osThreadNew(StartLEDBlinkTask, NULL, &LEDBlinkTask_attributes);

  /* creation of SDLogTask */
  SDLogTaskHandle = osThreadNew(StartSDLogTask, NULL, &SDLogTask_attributes);

  /* creation of debugPrintTask */
  debugPrintTaskHandle = osThreadNew(StartDebugPrintTask, NULL, &debugPrintTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;) {
    pca.hi2c = &hi2c1;
    pca.addr = (0x40 << 1);
    pca1.hi2c = &hi2c1;
    pca1.addr = ((0x40 + 1) << 1);
    IBUS_Init(&ibus, &huart3, &data);
    MPU6050_Init(&hi2c2);
    PCA9685_Init(&pca);
    PCA9685_Init(&pca1);
    vTaskDelete(NULL);
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartIBusTask */
/**
 * @brief Function implementing the IBusTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartIBusTask */
void StartIBusTask(void *argument)
{
  /* USER CODE BEGIN StartIBusTask */
  /* Infinite loop */
  for (;;) {
    osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
    osMutexAcquire(uartMutexHandle, osWaitForever);
    if (ibus.ready) {
      IBUS_Parse(&ibus);
    }
    osMutexRelease(uartMutexHandle);
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, ibus.buffer, IBUS_SIZE);
  }
  /* USER CODE END StartIBusTask */
}

/* USER CODE BEGIN Header_StartServoTask */
/**
 * @brief Function implementing the ServoTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartServoTask */
void StartServoTask(void *argument)
{
  /* USER CODE BEGIN StartServoTask */
  /* Infinite loop */
  for (;;) {
    int degree = (ibus.data->left_vertical - 1000) * 180.0f / 1000.0f;;
    for (int i = 0; i < 3; i++) {
      PCA9685_SetServoAngle(&pca, i, degree);
    }

    for (int i = 0; i < 4; i++) {
      PCA9685_SetServoAngle(&pca1, i, degree);
    }

    osDelay(1000);
  }
  /* USER CODE END StartServoTask */
}

/* USER CODE BEGIN Header_StartIMUTask */
/**
 * @brief Function implementing the IMUTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartIMUTask */
void StartIMUTask(void *argument)
{
  /* USER CODE BEGIN StartIMUTask */
  /* Infinite loop */
  for (;;) {
    MPU6050_ReadAll(&hi2c2, &mpu);
    osDelay(500);
  }
  /* USER CODE END StartIMUTask */
}

/* USER CODE BEGIN Header_StartLEDBlinkTask */
/**
 * @brief Function implementing the LEDBlinkTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLEDBlinkTask */
void StartLEDBlinkTask(void *argument)
{
  /* USER CODE BEGIN StartLEDBlinkTask */
  /* Infinite loop */
  for (;;) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    osDelay(1000);
  }
  /* USER CODE END StartLEDBlinkTask */
}

/* USER CODE BEGIN Header_StartSDLogTask */
/**
 * @brief Function implementing the SDLogTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartSDLogTask */
void StartSDLogTask(void *argument)
{
  /* USER CODE BEGIN StartSDLogTask */
    fr = f_mount(&fs, "", 1);
    if (fr != FR_OK) {
        vTaskDelete(NULL);
    }

    fr = f_open(&file, "log.txt", FA_WRITE | FA_OPEN_APPEND);
    if (fr != FR_OK) {
        vTaskDelete(NULL);
    }
      f_lseek(&file, f_size(&file));

  /* Infinite loop */
  for (;;) {
    osMutexAcquire(uartMutexHandle, osWaitForever);

      sprintf(logbuf,
        "Ch 1: %d Ch 2: %d Ch 3: %d Ch 4: %d Ch 5: %d Ch 6: %d Ch 7: %d Ch 8: %d Ch 9: %d Ch 10: %d | Accel: X%.2f Y%.2f Z%.2f | Gyro: X%.2f Y%.2f Z%.2f | Temp: %.2f\r\n", 
        ibus.data->left_horizontal, ibus.data->left_vertical, ibus.data->right_horizontal, ibus.data->right_vertical, 
        ibus.data->pot1, ibus.data->pot2, ibus.data->switch1, ibus.data->switch2, ibus.data->switch3, ibus.data->switch4,
        mpu.accel_x, mpu.accel_y, mpu.accel_z, mpu.gyro_x, mpu.gyro_y, mpu.gyro_z, mpu.temp
      );

      fr = f_write(&file, logbuf, strlen(logbuf), &bytes);

      f_sync(&file);

      osMutexRelease(uartMutexHandle);
    osDelay(1000);
  }
  /* USER CODE END StartSDLogTask */
}

/* USER CODE BEGIN Header_StartDebugPrintTask */
/**
* @brief Function implementing the debugPrintTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugPrintTask */
void StartDebugPrintTask(void *argument)
{
  /* USER CODE BEGIN StartDebugPrintTask */
  IBus_Data_t ibus_local_data = {1000};
  MPU6050_Data_t mpu_local_data = {0};
  /* Infinite loop */
  for(;;)
  {
        osMutexAcquire(uartMutexHandle, osWaitForever);
        if (ibus.data == NULL) {
          osMutexRelease(uartMutexHandle);
          osDelay(100);
          continue;
        }
      else  {
          ibus_local_data = *ibus.data; 
      }
      
      mpu_local_data = mpu;
      osMutexRelease(uartMutexHandle);
    printf("Ch 1: %d Ch 2: %d Ch 3: %d Ch 4: %d Ch 5: %d Ch 6: %d Ch 7: %d Ch 8: %d Ch 9: %d Ch 10: %d | Accel: X%.2f Y%.2f Z%.2f | Gyro: X%.2f Y%.2f Z%.2f | Temp: %.2f\r\n", 
        ibus_local_data.left_horizontal, ibus_local_data.left_vertical, ibus_local_data.right_horizontal, ibus_local_data.right_vertical, 
        ibus_local_data.pot1, ibus_local_data.pot2, ibus_local_data.switch1, ibus_local_data.switch2, ibus_local_data.switch3, ibus_local_data.switch4,
        mpu_local_data.accel_x, mpu_local_data.accel_y, mpu_local_data.accel_z, mpu_local_data.gyro_x, mpu_local_data.gyro_y, mpu_local_data.gyro_z, mpu_local_data.temp);
    osDelay(500);
  }
  /* USER CODE END StartDebugPrintTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

