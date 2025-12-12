/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LOG_SPI2_MOSI_Pin GPIO_PIN_1
#define LOG_SPI2_MOSI_GPIO_Port GPIOC
#define LOG_SPI2_MISO_Pin GPIO_PIN_2
#define LOG_SPI2_MISO_GPIO_Port GPIOC
#define CONNECTION_UART4_TX_Pin GPIO_PIN_0
#define CONNECTION_UART4_TX_GPIO_Port GPIOA
#define CONNECTION_UART4_RX_Pin GPIO_PIN_1
#define CONNECTION_UART4_RX_GPIO_Port GPIOA
#define DEBUG_USART_TX_Pin GPIO_PIN_2
#define DEBUG_USART_TX_GPIO_Port GPIOA
#define DEBUG_USART_RX_Pin GPIO_PIN_3
#define DEBUG_USART_RX_GPIO_Port GPIOA
#define LOG_SD_CS_Pin GPIO_PIN_4
#define LOG_SD_CS_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define LEG5_TOUCH_SENSOR_Pin GPIO_PIN_6
#define LEG5_TOUCH_SENSOR_GPIO_Port GPIOA
#define LEG4_TOUCH_SENSOR_Pin GPIO_PIN_7
#define LEG4_TOUCH_SENSOR_GPIO_Port GPIOA
#define LEG3_TOUCH_SENSOR_Pin GPIO_PIN_4
#define LEG3_TOUCH_SENSOR_GPIO_Port GPIOC
#define IBUS_USART3_RX_Pin GPIO_PIN_5
#define IBUS_USART3_RX_GPIO_Port GPIOC
#define MPU6050_I2C2_SCL_Pin GPIO_PIN_10
#define MPU6050_I2C2_SCL_GPIO_Port GPIOB
#define LOG_SDIO_SCK_Pin GPIO_PIN_13
#define LOG_SDIO_SCK_GPIO_Port GPIOB
#define LEG2_TOUCH_SENSOR_Pin GPIO_PIN_14
#define LEG2_TOUCH_SENSOR_GPIO_Port GPIOB
#define LEG1_TOUCH_SENSOR_Pin GPIO_PIN_15
#define LEG1_TOUCH_SENSOR_GPIO_Port GPIOB
#define LEG0_TOUCH_SENSOR_Pin GPIO_PIN_6
#define LEG0_TOUCH_SENSOR_GPIO_Port GPIOC
#define LED6_Pin GPIO_PIN_9
#define LED6_GPIO_Port GPIOC
#define LED5_Pin GPIO_PIN_8
#define LED5_GPIO_Port GPIOA
#define LED4_Pin GPIO_PIN_9
#define LED4_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_10
#define LED3_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define IBUS_USART3_TX_Pin GPIO_PIN_10
#define IBUS_USART3_TX_GPIO_Port GPIOC
#define MPU6050_I2C2_SDA_Pin GPIO_PIN_12
#define MPU6050_I2C2_SDA_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPS_USART1_TX_Pin GPIO_PIN_6
#define GPS_USART1_TX_GPIO_Port GPIOB
#define GPS_USART1_RX_Pin GPIO_PIN_7
#define GPS_USART1_RX_GPIO_Port GPIOB
#define PCA9685_I2C1_SCL_Pin GPIO_PIN_8
#define PCA9685_I2C1_SCL_GPIO_Port GPIOB
#define PCA9685_I2C1_SDA_Pin GPIO_PIN_9
#define PCA9685_I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define SD_SPI_HANDLE hspi2
#define SD_CS_GPIO_Port GPIOA
#define SD_CS_Pin GPIO_PIN_4

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
