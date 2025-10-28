#include "pca9685.h"

HAL_StatusTypeDef PCA9685_WriteReg(PCA9685_Handle_t *pca, uint8_t reg, uint8_t data) {
    return HAL_I2C_Mem_Write(pca->hi2c, pca->addr, reg, 1, &data, 1, TIMEOUT);
}

HAL_StatusTypeDef PCA9685_ReadReg(PCA9685_Handle_t *pca, uint8_t reg, uint8_t *data) {
    return HAL_I2C_Mem_Read(pca->hi2c, pca->addr, reg, 1, data, 1, TIMEOUT);
}

HAL_StatusTypeDef PCA9685_WriteBurst(PCA9685_Handle_t *pca, uint8_t reg, uint8_t *data, uint16_t len) {
    return HAL_I2C_Mem_Write(pca->hi2c, pca->addr, reg, 1, data, len, TIMEOUT);
}

HAL_StatusTypeDef PCA9685_Init(PCA9685_Handle_t *pca) {
    uint8_t data;
    if (PCA9685_ReadReg(pca, PCA9685_MODE1_REG, &data) != HAL_OK) {
        return HAL_ERROR;
    }

    if (PCA9685_WriteReg(pca, PCA9685_MODE1_REG, (data | 0x10)) != HAL_OK) {
        return HAL_ERROR;
    }
    HAL_Delay(1);

    if (PCA9685_SetPWMFreq(pca, FREQUENCY) != HAL_OK) {
        return HAL_ERROR;
    }

    if (PCA9685_WriteReg(pca, PCA9685_MODE1_REG, ((data & ~0x10) | 0xA0)) != HAL_OK) {
        return HAL_ERROR;
    }
    
    HAL_Delay(5);

    return HAL_OK;
}

HAL_StatusTypeDef PCA9685_SetPWMFreq(PCA9685_Handle_t *pca, float freq) {
    uint8_t prescale;
    float prescaleval = 25000000.0f / (4096.0f * freq) - 1.0f;
    prescale = (uint8_t)(prescaleval + 0.5f);

    return PCA9685_WriteReg(pca, PCA9685_PRE_SCALE_REG, prescale);
}

HAL_StatusTypeDef PCA9685_Wake(PCA9685_Handle_t *pca) {
    uint8_t data;
    if (PCA9685_ReadReg(pca, PCA9685_MODE1_REG, &data) != HAL_OK) {
        return HAL_ERROR;
    }

    data &= ~0x10;
    if (PCA9685_WriteReg(pca, PCA9685_MODE1_REG, data) != HAL_OK) {
        return HAL_ERROR;
    }

    HAL_Delay(1);
    data |= 0x80;
    return PCA9685_WriteReg(pca, PCA9685_MODE1_REG, data);
}

HAL_StatusTypeDef PCA9685_Sleep(PCA9685_Handle_t *pca) {
    uint8_t data;
    if (PCA9685_ReadReg(pca, PCA9685_MODE1_REG, &data) != HAL_OK) {
        return HAL_ERROR;
    }

    data = (data | 0x10) & ~0x80;

    return PCA9685_WriteReg(pca, PCA9685_MODE1_REG, data);
}

HAL_StatusTypeDef PCA9685_SetPWM(PCA9685_Handle_t *pca, uint8_t channel, uint16_t on_val, uint16_t off_val) {
    uint8_t buffer[4];
    buffer[0] = on_val;
    buffer[1] = on_val >> 8;
    buffer[2] = off_val;
    buffer[3] = off_val >> 8;

    return PCA9685_WriteBurst(pca, PCA9685_LEDn_ON_L(channel), buffer, 4);
}

HAL_StatusTypeDef PCA9685_SetDutyCycle(PCA9685_Handle_t *pca, uint8_t channel, uint16_t duty) {
    return PCA9685_SetPWM(pca, channel, 0, duty);
}

HAL_StatusTypeDef PCA9685_SetServoAngle(PCA9685_Handle_t *pca, uint8_t channel, float angle) {
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;

    float pulse_min = 1.0f;
    float pulse_max = 2.0f;
    float pulse_width = pulse_min + (angle / 180.0f) * (pulse_max - pulse_min);

    float period_ms = 1000.0f / FREQUENCY;
    uint16_t count = (uint16_t)((pulse_width / period_ms) * 4096.0f);

    return PCA9685_SetPWM(pca, channel, 0, count);
}

uint8_t PCA9685_ReadID(PCA9685_Handle_t *pca) {
    uint8_t data;
    if (PCA9685_ReadReg(pca, PCA9685_MODE1_REG, &data) == HAL_OK) {
        return pca->addr;
    }
    return 0;
}
