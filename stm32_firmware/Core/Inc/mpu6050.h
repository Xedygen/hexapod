#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "main.h"
#include "stm32f4xx_hal.h"

#define MPU6050_ADDR 0x68 << 1

#define MPU6050_ACCEL_CONFIG_REG 0x1C
#define MPU6050_GYRO_CONFIG_REG 0x1B
#define MPU6050_ACCEL_XOUT_H_REG 0x3B
#define MPU6050_TEMP_OUT_H_REG 0x41
#define MPU6050_GYRO_XOUT_H_REG 0x43
#define MPU6050_PWR_MGMT_1_REG 0x6B
#define MPU6050_WHO_AM_I_REG 0x75

#define TIMEOUT 100

typedef enum {
    G2  = 16384,
    G4  = 8192,
    G8  = 4096,
    G16 = 2048
}Accel_Sensitivity;

typedef enum { // divide by 10
    G250  = 1310,
    G500  = 655,
    G1000 = 328,
    G2000 = 164
}Gyro_Sensitivity;

typedef struct {
    float Accel_x;
    float Accel_y;
    float Accel_z;
    float Temp;
    float Gyro_x;
    float Gyro_y;
    float Gyro_z;
} MPU6050_Data_t;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Accel_Config(I2C_HandleTypeDef *hi2c, uint8_t val);
HAL_StatusTypeDef MPU6050_Gyro_Config(I2C_HandleTypeDef *hi2c, uint8_t val);
HAL_StatusTypeDef MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadAll(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);

#endif

