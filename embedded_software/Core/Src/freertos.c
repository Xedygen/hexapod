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

#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>

#include "dma.h"
#include "fatfs.h"
#include "gpio.h"
#include "i2c.h"
#include "ibus.h"
#include "mpu6050.h"
#include "pca9685.h"
#include "spi.h"
#include "usart.h"

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

char logbuf[64];
IBUS_Handle_t ibus;
IBus_Data_t data;
MPU6050_Data_t mpu;
PCA9685_Handle_t pca;
PCA9685_Handle_t pca1;

FATFS fs;
FRESULT fr;
FIL file;
UINT bytes;

extern IWDG_HandleTypeDef hiwdg;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for IBusTask */
osThreadId_t IBusTaskHandle;
const osThreadAttr_t IBusTask_attributes = {
    .name = "IBusTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for ServoTask */
osThreadId_t ServoTaskHandle;
const osThreadAttr_t ServoTask_attributes = {
    .name = "ServoTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for IMUTask */
osThreadId_t IMUTaskHandle;
const osThreadAttr_t IMUTask_attributes = {
    .name = "IMUTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for LEDBlinkTask */
osThreadId_t LEDBlinkTaskHandle;
const osThreadAttr_t LEDBlinkTask_attributes = {
    .name = "LEDBlinkTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for SDLogTask */
osThreadId_t SDLogTaskHandle;
const osThreadAttr_t SDLogTask_attributes = {
    .name = "SDLogTask",
    .stack_size = 256 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for debugPrintTask */
osThreadId_t debugPrintTaskHandle;
const osThreadAttr_t debugPrintTask_attributes = {
    .name = "debugPrintTask",
    .stack_size = 1024 * 4,
    .priority = (osPriority_t)osPriorityBelowNormal,
};
/* Definitions for GPSTask */
osThreadId_t GPSTaskHandle;
const osThreadAttr_t GPSTask_attributes = {
    .name = "GPSTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for Interface */
osThreadId_t InterfaceHandle;
const osThreadAttr_t Interface_attributes = {
    .name = "Interface",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for TouchSensor */
osThreadId_t TouchSensorHandle;
const osThreadAttr_t TouchSensor_attributes = {
    .name = "TouchSensor",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for WatchDogTask */
osThreadId_t WatchDogTaskHandle;
const osThreadAttr_t WatchDogTask_attributes = {
    .name = "WatchDogTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityLow,
};
/* Definitions for uartMutex */
osMutexId_t uartMutexHandle;
const osMutexAttr_t uartMutex_attributes = {.name = "uartMutex"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void* argument);
void StartIBusTask(void* argument);
void StartServoTask(void* argument);
void StartIMUTask(void* argument);
void StartLEDBlinkTask(void* argument);
void StartSDLogTask(void* argument);
void StartDebugPrintTask(void* argument);
void StartGPSTask(void* argument);
void StartInterface(void* argument);
void StartTouchSensor(void* argument);
void StartWatchDogTask(void* argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char* pcTaskName);
void vApplicationMallocFailedHook(void);

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char* pcTaskName) {
  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
  called if a stack overflow is detected. */

  taskDISABLE_INTERRUPTS();
  for (;;) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    osDelay(100);
  }
}
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void) {
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created. It is also called by various parts of the
  demo application. If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */

  taskDISABLE_INTERRUPTS();
  for (;;) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_9);
    osDelay(100);
  }
}
/* USER CODE END 5 */

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
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of IBusTask */
  IBusTaskHandle = osThreadNew(StartIBusTask, NULL, &IBusTask_attributes);

  /* creation of ServoTask */
  ServoTaskHandle = osThreadNew(StartServoTask, NULL, &ServoTask_attributes);

  /* creation of IMUTask */
  IMUTaskHandle = osThreadNew(StartIMUTask, NULL, &IMUTask_attributes);

  /* creation of LEDBlinkTask */
  LEDBlinkTaskHandle =
      osThreadNew(StartLEDBlinkTask, NULL, &LEDBlinkTask_attributes);

  /* creation of SDLogTask */
  SDLogTaskHandle = osThreadNew(StartSDLogTask, NULL, &SDLogTask_attributes);

  /* creation of debugPrintTask */
  debugPrintTaskHandle =
      osThreadNew(StartDebugPrintTask, NULL, &debugPrintTask_attributes);

  /* creation of GPSTask */
  GPSTaskHandle = osThreadNew(StartGPSTask, NULL, &GPSTask_attributes);

  /* creation of Interface */
  InterfaceHandle = osThreadNew(StartInterface, NULL, &Interface_attributes);

  /* creation of TouchSensor */
  TouchSensorHandle =
      osThreadNew(StartTouchSensor, NULL, &TouchSensor_attributes);

  /* creation of WatchDogTask */
  WatchDogTaskHandle =
      osThreadNew(StartWatchDogTask, NULL, &WatchDogTask_attributes);

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
void StartDefaultTask(void* argument) {
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
void StartIBusTask(void* argument) {
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
void StartServoTask(void* argument) {
  /* USER CODE BEGIN StartServoTask */
  /* Infinite loop */
  for (;;) {
    // int degree = (ibus.data->left_vertical - 1000) * 180.0f / 1000.0f;;
    PCA9685_SetServoAngle(&pca, 0, 30);
    PCA9685_SetServoAngle(&pca, 1, 30);
    PCA9685_SetServoAngle(&pca, 2, 30);

    osDelay(1000);

    PCA9685_SetServoAngle(&pca, 0, -30);
    PCA9685_SetServoAngle(&pca, 1, -30);
    PCA9685_SetServoAngle(&pca, 2, -30);

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
void StartIMUTask(void* argument) {
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
void StartLEDBlinkTask(void* argument) {
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
void StartSDLogTask(void* argument) {
  /* USER CODE BEGIN StartSDLogTask */
  fr = f_mount(&fs, "0:", 1);
  if (fr != FR_OK) {
    for (;;);
  }

  fr = f_open(&file, "0:/log.txt", FA_WRITE | FA_OPEN_APPEND | FA_OPEN_ALWAYS);
  if (fr != FR_OK) {
    for (;;);
  }

  // ---- Seek to end ----
  f_lseek(&file, f_size(&file));

  /* Infinite loop */
  for (;;) {
    int len = snprintf(logbuf, sizeof(logbuf), "Logging something...\r\n");

    fr = f_write(&file, logbuf, len, &bytes);
    if (fr != FR_OK || bytes != len) {
      for (;;);
    }

    f_sync(&file);

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
void StartDebugPrintTask(void* argument) {
  /* USER CODE BEGIN StartDebugPrintTask */
  /* Infinite loop */
  for (;;) {
    printf("System is working...");
    osDelay(1000);
  }
  /* USER CODE END StartDebugPrintTask */
}

/* USER CODE BEGIN Header_StartGPSTask */
/**
 * @brief Function implementing the GPSTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGPSTask */
void StartGPSTask(void* argument) {
  /* USER CODE BEGIN StartGPSTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartGPSTask */
}

/* USER CODE BEGIN Header_StartInterface */
/**
 * @brief Function implementing the Interface thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInterface */
void StartInterface(void* argument) {
  /* USER CODE BEGIN StartInterface */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartInterface */
}

/* USER CODE BEGIN Header_StartTouchSensor */
/**
 * @brief Function implementing the TouchSensor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTouchSensor */
void StartTouchSensor(void* argument) {
  /* USER CODE BEGIN StartTouchSensor */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTouchSensor */
}

/* USER CODE BEGIN Header_StartWatchDogTask */
/**
 * @brief Function implementing the WatchDogTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartWatchDogTask */
void StartWatchDogTask(void* argument) {
  /* USER CODE BEGIN StartWatchDogTask */
  /* Infinite loop */
  for (;;) {
    HAL_IWDG_Refresh(&hiwdg);
    osDelay(200);
  }
  /* USER CODE END StartWatchDogTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
