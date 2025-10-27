#ifndef _SD_H_
#define _SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"

#define CMD0    0x40
#define CMD(n)    (CMD0 + n)



HAL_StatusTypeDef SD_SPITransmitReceive(uint8_t data);
HAL_StatusTypeDef SD_CSControl(uint8_t state);
HAL_StatusTypeDef SD_SetSspeed(uint8_t speed);
HAL_StatusTypeDef SD_CardInit();
HAL_StatusTypeDef SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);
HAL_StatusTypeDef SD_ReadSingleBlock(uint32_t blockAddr, uint8_t *buffer);
HAL_StatusTypeDef SD_WriteSingleBlock(uint32_t blockAddr, const uint8_t *buffer);

#ifdef __cplusplus
}
#endif

#endif