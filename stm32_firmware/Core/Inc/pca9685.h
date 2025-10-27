#ifndef __PCA9685_H__
#define __PCA9685_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"

#define PCA9685_MODE1_REG           0x00
#define PCA9685_MODE2_REG           0x01
#define PCA9685_SUBADDRn_REG(n)    (0x01 + n)
#define PCA9685_ALLCALL_ADDR_REG    0x05
#define PCA9685_LED0_ON_L_REG       0x06
#define PCA9685_ALL_LED_ON_L_REG    0xFA
#define PCA9685_PRE_SCALE_REG       0xFE
#define PCA9685_TEST_MODE_REG       0xFF
#define PCA9685_CHANNEL             4
#define PCA9685_LEDn_ON_L(n)        (PCA9685_LED0_ON_L_REG + PCA9685_CHANNEL * (n))

#define TIMEOUT 100
#define FREQUENCY 50

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t addr;
    float freq;
} PCA9685_Handle_t;

HAL_StatusTypeDef PCA9685_WriteReg(PCA9685_Handle_t *pca, uint8_t reg, uint8_t data);
HAL_StatusTypeDef PCA9685_ReadReg(PCA9685_Handle_t *pca, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef PCA9685_WriteBurst(PCA9685_Handle_t *pca, uint8_t reg, uint8_t *data, uint16_t len);
HAL_StatusTypeDef PCA9685_Init(PCA9685_Handle_t *pca);
HAL_StatusTypeDef PCA9685_SetPWMFreq(PCA9685_Handle_t *pca, float freq);
HAL_StatusTypeDef PCA9685_Wake(PCA9685_Handle_t *pca);
HAL_StatusTypeDef PCA9685_Sleep(PCA9685_Handle_t *pca);
HAL_StatusTypeDef PCA9685_SetPWM(PCA9685_Handle_t *pca, uint8_t channel, uint16_t on_val, uint16_t off_val);
HAL_StatusTypeDef PCA9685_SetDutyCycle(PCA9685_Handle_t *pca, uint8_t channel, uint16_t duty);
HAL_StatusTypeDef PCA9685_SetServoAngle(PCA9685_Handle_t *pca, uint8_t channel, float angle);
uint8_t PCA9685_ReadID(PCA9685_Handle_t *pca);

#ifdef __cplusplus
}
#endif

#endif