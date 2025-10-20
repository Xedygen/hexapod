#include "mpu6050.h"

//these settings are default values
Accel_Sensitivity accelSens = G2;
Gyro_Sensitivity gyroSens = G250;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t flag = 0;
    uint8_t pwr = 0x00;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    if (flag != 0x68) {
        return HAL_ERROR;
    }

    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_Accel_Config(I2C_HandleTypeDef *hi2c, uint8_t val) {
    uint8_t current;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &current, 1, TIMEOUT) != HAL_OK)
        return HAL_ERROR;

    current = (current & ~0x18) | (val << 3);

    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_ACCEL_CONFIG_REG, 1, &current, 1, TIMEOUT) != HAL_OK)
        return HAL_ERROR;

    switch (val) {
        case 0: accelSens = G2;  break;
        case 1: accelSens = G4;  break;
        case 2: accelSens = G8;  break;
        case 3: accelSens = G16; break;
        default: accelSens = G2; break;
    }

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_Gyro_Config(I2C_HandleTypeDef *hi2c, uint8_t val) {
    uint8_t current;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &current, 1, TIMEOUT) != HAL_OK)
        return HAL_ERROR;

    current = (current & ~0x18) | (val << 3);

    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_GYRO_CONFIG_REG, 1, &current, 1, TIMEOUT) != HAL_OK)
        return HAL_ERROR;

    switch (val) {
        case 0: gyroSens = G250;  break;
        case 1: gyroSens = G500;  break;
        case 2: gyroSens = G1000; break;
        case 3: gyroSens = G2000; break;
        default: gyroSens = G250; break;
    }

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadAccel(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[6];

    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buffer, 6, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->Accel_x = ((float)((int16_t)((buffer[0] << 8) | buffer[1]))) / accelSens;
    data->Accel_y = ((float)((int16_t)((buffer[2] << 8) | buffer[3]))) / accelSens;
    data->Accel_z = ((float)((int16_t)((buffer[4] << 8) | buffer[5]))) / accelSens;
    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[2];
    
    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_TEMP_OUT_H_REG, 1, buffer, 2, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->Temp = ((int16_t)((buffer[0] << 8) | buffer[1])) / 340.0f + 36.53f;

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[6];

    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_GYRO_XOUT_H_REG, 1, buffer, 6, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->Gyro_x = ((float)((int16_t)((buffer[0] << 8) | buffer[1]))) / (gyroSens / 10.0f);
    data->Gyro_y = ((float)((int16_t)((buffer[2] << 8) | buffer[3]))) / (gyroSens / 10.0f);
    data->Gyro_z = ((float)((int16_t)((buffer[4] << 8) | buffer[5]))) / (gyroSens / 10.0f);

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadAll(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    MPU6050_ReadAccel(hi2c, data);
    MPU6050_ReadTemp(hi2c, data);
    MPU6050_ReadGyro(hi2c, data);

    return HAL_OK;
}
