#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "main.h"
#include "stm32f4xx_hal.h"
#include <math.h>

#define MPU6050_ADDR 0x68 << 1

#define MPU6050_ACCEL_CONFIG_REG    0x1C
#define MPU6050_GYRO_CONFIG_REG     0x1B
#define MPU6050_ACCEL_XOUT_H_REG    0x3B
#define MPU6050_TEMP_OUT_H_REG      0x41
#define MPU6050_GYRO_XOUT_H_REG     0x43
#define MPU6050_PWR_MGMT_1_REG      0x6B
#define MPU6050_WHO_AM_I_REG        0x75
#define MPU6050_FIFO_ENABLE_REG     0x23
#define MPU6050_INT_ENABLE_REG      0x38
#define MPU6050_INT_STATUS_REG      0x3A
#define MPU6050_CONFIG_REG          0x1A
#define MPU6050_USER_CTRL_REG       0x6A
#define MPU6050_SMPRT_DIV_REG       0x19
#define MPU6050_ACCEL_XOFFS_H_REG   0x06
#define MPU6050_ACCEL_YOFFS_H_REG   0x08
#define MPU6050_ACCEL_ZOFFS_H_REG   0x0A
#define MPU6050_GYRO_XOFFS_H_REG    0X13
#define MPU6050_GYRO_YOFFS_H_REG    0X15
#define MPU6050_GYRO_ZOFFS_H_REG    0X17

#define TIMEOUT 100
#define SAMPLES 500 //num of samples for offset calc

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

typedef enum {
    INTERNAL = 0,
    GYRO_X = 1,
    GYRO_Y = 2,
    GYRO_Z = 3,
    EXTERNAL32768 = 4,
    EXTERNAL192 = 5,
    STOP = 7
}Clock_Source;

typedef struct {
    int16_t raw_accel_x, raw_accel_y, raw_accel_z;
    int16_t raw_gyro_x, raw_gyro_y, raw_gyro_z;
    float accel_x, accel_y, accel_z;
    float temp;
    float gyro_x, gyro_y, gyro_z;
    float roll, pitch, yaw;
} MPU6050_Data_t;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Accel_Config(I2C_HandleTypeDef *hi2c, uint8_t val);
HAL_StatusTypeDef MPU6050_Gyro_Config(I2C_HandleTypeDef *hi2c, uint8_t val);
HAL_StatusTypeDef MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_ReadAll(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_EnableDMP(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_ReadDMP(I2C_HandleTypeDef *hi2c, float *pitch, float *roll, float *yaw);
HAL_StatusTypeDef MPU6050_Sleep(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Wake(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_SetClockSource(I2C_HandleTypeDef *hi2c, uint8_t source);
HAL_StatusTypeDef MPU6050_SetAccelOffset(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_SetGyroOffset(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_Calibrate(MPU6050_Data_t *data, uint16_t samples);
HAL_StatusTypeDef MPU6050_EnableFIFO(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_DisableFIFO(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_ReadFIFO(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data);
HAL_StatusTypeDef MPU6050_EnableInterrupt(I2C_HandleTypeDef *hi2c, uint8_t int_mask);
HAL_StatusTypeDef MPU6050_DisableInterrupt(I2C_HandleTypeDef *hi2c);
uint8_t MPU6050_ReadInterruptStatus(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MPU6050_SetDLPF(I2C_HandleTypeDef *hi2c, uint8_t setting);
uint8_t MPU6050_ReadID(I2C_HandleTypeDef *hi2c);

#endif

