#include "mpu6050.h"

//these settings are default values
Accel_Sensitivity accelSens = G2;
Gyro_Sensitivity gyroSens = G250;
Clock_Source clockSource = INTERNAL;

HAL_StatusTypeDef MPU6050_Init(I2C_HandleTypeDef *hi2c) {
    uint8_t flag = 0;
    uint8_t pwr = 0x00;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    if (flag != 0x68) {
        return HAL_ERROR;
    }

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT);
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

    data->raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);
    data->accel_x = ((float)(data->raw_accel_x)) / accelSens;
    data->accel_y = ((float)(data->raw_accel_y)) / accelSens;
    data->accel_z = ((float)(data->raw_accel_z)) / accelSens;
    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadTemp(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[2];
    
    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_TEMP_OUT_H_REG, 1, buffer, 2, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->temp = ((int16_t)((buffer[0] << 8) | buffer[1])) / 340.0f + 36.53f;

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadGyro(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[6];

    if(HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_GYRO_XOUT_H_REG, 1, buffer, 6, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->raw_gyro_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->raw_gyro_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->raw_gyro_z = (int16_t)((buffer[4] << 8) | buffer[5]);
    data->gyro_x = ((float)(data->raw_gyro_x)) / (gyroSens / 10.0f);
    data->gyro_y = ((float)(data->raw_gyro_y)) / (gyroSens / 10.0f);
    data->gyro_z = ((float)(data->raw_gyro_z)) / (gyroSens / 10.0f);

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_ReadAll(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t buffer[14];
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buffer, 14, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    data->raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);
    data->accel_x = ((float)(data->raw_accel_x)) / accelSens;
    data->accel_y = ((float)(data->raw_accel_y)) / accelSens;
    data->accel_z = ((float)(data->raw_accel_z)) / accelSens;
    data->temp = ((int16_t)((buffer[6] << 8) | buffer[7])) / 340.0f + 36.53f;
    data->raw_gyro_x = (int16_t)((buffer[8] << 8) | buffer[9]);
    data->raw_gyro_y = (int16_t)((buffer[10] << 8) | buffer[11]);
    data->raw_gyro_z = (int16_t)((buffer[12] << 8) | buffer[13]);
    data->gyro_x = ((float)(data->raw_gyro_x)) / (gyroSens / 10.0f);
    data->gyro_y = ((float)(data->raw_gyro_y)) / (gyroSens / 10.0f);
    data->gyro_z = ((float)(data->raw_gyro_z)) / (gyroSens / 10.0f);

    return HAL_OK;
}

//HAL_StatusTypeDef MPU6050_EnableDMP(I2C_HandleTypeDef *hi2c) {}
//HAL_StatusTypeDef MPU6050_ReadDMP(I2C_HandleTypeDef *hi2c, float *pitch, float *roll, float *yaw) {}

HAL_StatusTypeDef MPU6050_Sleep(I2C_HandleTypeDef *hi2c) {
    uint8_t flag = 0;
    uint8_t pwr;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    if (flag != 0x68) {
        return HAL_ERROR;
    }

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT) !=HAL_OK) {
        return HAL_ERROR;
    }

    pwr |= 0x40;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_Wake(I2C_HandleTypeDef *hi2c) {
    uint8_t flag = 0;
    uint8_t pwr;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    if (flag != 0x68) {
        return HAL_ERROR;
    }

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT) !=HAL_OK) {
        return HAL_ERROR;
    }

    pwr &= ~0x40;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT);
}

//As a source value only 0, 1, 2, 3, 4, 5, 7 should be used other values will break the sensor.
HAL_StatusTypeDef MPU6050_SetClockSource(I2C_HandleTypeDef *hi2c, uint8_t source) {
    uint8_t flag = 0;
    uint8_t pwr;

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }

    if (flag != 0x68) {
        return HAL_ERROR;
    }

    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT) !=HAL_OK) {
        return HAL_ERROR;
    }

    pwr = (pwr & ~0x07) | (source & 0x07);

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_PWR_MGMT_1_REG, 1, &pwr, 1, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_SetAccelOffset(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[6];
    int32_t sum_x = 0, sum_y = 0, sum_z = 0;
    
    for (int i = 0; i < SAMPLES; i++) {
        if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_ACCEL_XOUT_H_REG, 1, buffer, 6, TIMEOUT) != HAL_OK) {
            return HAL_ERROR;
        }
        sum_x += (int16_t)((buffer[0] << 8) | buffer[1]);
        sum_y += (int16_t)((buffer[2] << 8) | buffer[3]);
        sum_z += (int16_t)((buffer[4] << 8) | buffer[5]);
        HAL_Delay(2);
    }

    float avg_x = (float)sum_x / SAMPLES;
    float avg_y = (float)sum_y / SAMPLES;
    float avg_z = (float)sum_z / SAMPLES;

    float magnitude = sqrtf(avg_x * avg_x + avg_y * avg_y + avg_z * avg_z);
    if (magnitude < 8000.0f || magnitude > 20000.0f) {
        return HAL_ERROR;
    }

    float offset_x, offset_y, offset_z;
    if (fabsf(avg_x) > fabsf(avg_y) && fabsf(avg_x) > fabsf(avg_z)) {
        float expected_g = (avg_x > 0) ? RAW_1G_EQUIVALENT : -RAW_1G_EQUIVALENT;
        offset_x = avg_x - expected_g; 
        offset_y = avg_y - 0.0f;       
        offset_z = avg_z - 0.0f;
    } else if (fabsf(avg_y) > fabsf(avg_z)) {
        float expected_g = (avg_y > 0) ? RAW_1G_EQUIVALENT : -RAW_1G_EQUIVALENT;
        offset_x = avg_x - 0.0f;
        offset_y = avg_y - expected_g;
        offset_z = avg_z - 0.0f;
    } else {
        float expected_g = (avg_z > 0) ? RAW_1G_EQUIVALENT : -RAW_1G_EQUIVALENT;
        offset_x = avg_x - 0.0f;
        offset_y = avg_y - 0.0f;
        offset_z = avg_z - expected_g;
    }

    int16_t x_offset_reg = (int16_t)roundf(offset_x / 4.0f);
    int16_t y_offset_reg = (int16_t)roundf(offset_y / 4.0f);
    int16_t z_offset_reg = (int16_t)roundf(offset_z / 4.0f);
    uint8_t data[6];
    data[0] = (x_offset_reg >> 8) & 0xFF;
    data[1] = (x_offset_reg & 0xFC); 
    data[2] = (y_offset_reg >> 8) & 0xFF;
    data[3] = (y_offset_reg & 0xFC);
    data[4] = (z_offset_reg >> 8) & 0xFF;
    data[5] = (z_offset_reg & 0xFC);

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_ACCEL_XOFFS_H_REG, 1, data, 6, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_SetGyroOffset(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[6];
    int32_t sensor_offset_x = 0, sensor_offset_y = 0, sensor_offset_z = 0;
    for (int i = 0; i < SAMPLES; i++) {
        if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_GYRO_XOUT_H_REG, 1, buffer, 6, TIMEOUT) != HAL_OK) {
            return HAL_ERROR;
        }
        sensor_offset_x += (int16_t)((buffer[0] << 8) | buffer[1]);
        sensor_offset_y += (int16_t)((buffer[2] << 8) | buffer[3]);
        sensor_offset_z += (int16_t)((buffer[4] << 8) | buffer[5]);
        HAL_Delay(2);
    }

     float avg_x= (float)sensor_offset_x / SAMPLES;
     float avg_y= (float)sensor_offset_y / SAMPLES;
     float avg_z= (float)sensor_offset_z / SAMPLES;

    int16_t x_offset = -1 * (int16_t)roundf(avg_x);
    int16_t y_offset = -1 * (int16_t)roundf(avg_y);
    int16_t z_offset = -1 * (int16_t)roundf(avg_z);

    uint8_t data[6];
    data[0] = (x_offset >> 8) & 0xFF;
    data[1] = x_offset & 0xFF;
    data[2] = (y_offset >> 8) & 0xFF;
    data[3] = y_offset & 0xFF;
    data[4] = (z_offset >> 8) & 0xFF;
    data[5] = z_offset & 0xFF;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_GYRO_XOFFS_H_REG, 1, &data[0], 2, TIMEOUT) ||
           HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_GYRO_YOFFS_H_REG, 1, &data[2], 2, TIMEOUT) ||
           HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_GYRO_ZOFFS_H_REG, 1, &data[4], 2, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_Calibrate(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data, uint16_t samples) {
    uint8_t accel;
    uint8_t gyro;

    switch (accelSens) {
        case G2: accel = 0;break; 
        case G4: accel = 1;break;
        case G8: accel = 2;break;  
        case G16: accel = 3;break;
        default: accel = 0;break;
    }

    switch (gyroSens) {
        case G250: gyro = 0;break;   
        case G500: gyro = 1;break;
        case G1000: gyro = 2;break;   
        case G2000: gyro = 3;break;
        default: gyro = 0;break;
    }

    if (MPU6050_SetDLPF(hi2c, 0) != HAL_OK) return HAL_ERROR;
    if (MPU6050_Accel_Config(hi2c, 0) != HAL_OK) return HAL_ERROR;
    if (MPU6050_Gyro_Config(hi2c, 0) != HAL_OK) return HAL_ERROR;
    HAL_Delay(100);

    if (MPU6050_SetAccelOffset(hi2c) != HAL_OK) return HAL_ERROR;
    if (MPU6050_SetGyroOffset(hi2c) != HAL_OK) return HAL_ERROR;
    if (MPU6050_Accel_Config(hi2c, accel) != HAL_OK) return HAL_ERROR;
    if (MPU6050_Gyro_Config(hi2c, gyro) != HAL_OK) return HAL_ERROR;

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_EnableFIFO(I2C_HandleTypeDef *hi2c) {
    uint8_t val = 0xF8;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_FIFO_ENABLE_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    val &= ~0x40;
    val |= 0x04;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    val |= 0x40;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_DisableFIFO(I2C_HandleTypeDef *hi2c) {
    uint8_t val = 0x00;
    if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_FIFO_ENABLE_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    val &= ~0x40;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &val, 1, TIMEOUT);
}

HAL_StatusTypeDef MPU6050_ReadFIFO(I2C_HandleTypeDef *hi2c, MPU6050_Data_t *data) {
    uint8_t count_buffer[2];
    uint16_t count;
    uint8_t buffer[14];
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_FIFO_COUNT_REG, 1, count_buffer, 2, TIMEOUT) !=HAL_OK) {
        return HAL_ERROR;
    }
    count = count_buffer[0] << 8 | count_buffer[1];
    if (count >= 14) {
        if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_FIFO_DATA_REG, 1, buffer, 14, TIMEOUT) != HAL_OK) {
            return HAL_ERROR;
        }
        data->raw_accel_x = (int16_t)((buffer[0] << 8) | buffer[1]);
        data->raw_accel_y = (int16_t)((buffer[2] << 8) | buffer[3]);
        data->raw_accel_z = (int16_t)((buffer[4] << 8) | buffer[5]);
        data->accel_x = ((float)(data->raw_accel_x)) / accelSens;
        data->accel_y = ((float)(data->raw_accel_y)) / accelSens;
        data->accel_z = ((float)(data->raw_accel_z)) / accelSens;
        data->temp = ((int16_t)((buffer[6] << 8) | buffer[7])) / 340.0f + 36.53f;
        data->raw_gyro_x = (int16_t)((buffer[8] << 8) | buffer[9]);
        data->raw_gyro_y = (int16_t)((buffer[10] << 8) | buffer[11]);
        data->raw_gyro_z = (int16_t)((buffer[12] << 8) | buffer[13]);
        data->gyro_x = ((float)(data->raw_gyro_x)) / (gyroSens / 10.0f);
        data->gyro_y = ((float)(data->raw_gyro_y)) / (gyroSens / 10.0f);
        data->gyro_z = ((float)(data->raw_gyro_z)) / (gyroSens / 10.0f);
    }

    return HAL_OK;
}

//Only checks fifo interrupt.
HAL_StatusTypeDef MPU6050_EnableInterrupt(I2C_HandleTypeDef *hi2c) {
    uint8_t fifo;
    uint8_t interrupt;
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_USER_CTRL_REG, 1, &fifo, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    fifo &= 0x40;

    if (fifo == 0x40) {
        if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_INT_ENABLE_REG, 1, &interrupt, 1, TIMEOUT) != HAL_OK) {
            return HAL_ERROR;
        }
        interrupt |= 0x10;
        if (HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_INT_ENABLE_REG, 1, &interrupt, 1, TIMEOUT) != HAL_OK) {
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}

HAL_StatusTypeDef MPU6050_DisableInterrupt(I2C_HandleTypeDef *hi2c) {
    uint8_t fifo = 0x00;

    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_INT_ENABLE_REG, 1, &fifo, 1, TIMEOUT);
}

uint8_t MPU6050_ReadInterruptStatus(I2C_HandleTypeDef *hi2c) {
    uint8_t interrupt;
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_INT_STATUS_REG, 1, &interrupt, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    return interrupt;
}

//DLPF_CFG | Accelerometer             | Gyroscope
//         | Bandwidth (Hz) Delay (ms) | Bandwidth (Hz) Delay (ms) Fs (kHz)
//    0    | 260            0          | 256            0.98       8
//    1    | 184            2.0        | 188            1.9        1
//    2    | 94             3.0        | 98             2.8        1
//    3    | 44             4.9        | 42             4.8        1
//    4    | 21             8.5        | 20             8.3        1
//    5    | 10             13.8       | 10             13.4       1
//    6    | 5              19.0       | 5              18.6       1
//    7    | RESERVED                  | RESERVED                  8
HAL_StatusTypeDef MPU6050_SetDLPF(I2C_HandleTypeDef *hi2c, uint8_t setting) {
    uint8_t byte;
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_CONFIG_REG, 1, &byte, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    byte = (byte & 0xF8) | setting;
    return HAL_I2C_Mem_Write(hi2c, MPU6050_ADDR, MPU6050_CONFIG_REG, 1, &byte, 1, TIMEOUT);
}

uint8_t MPU6050_ReadID(I2C_HandleTypeDef *hi2c) {
    uint8_t flag;
    if (HAL_I2C_Mem_Read(hi2c, MPU6050_ADDR, MPU6050_WHO_AM_I_REG, 1, &flag, 1, TIMEOUT) != HAL_OK) {
        return HAL_ERROR;
    }
    return flag;
}