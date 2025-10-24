#ifndef _SD_H_
#define _SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"

#define CMD0    0x40
#define CMD1    (CMD0 + 1)
#define CMD6    (CMD0 + 6)
#define CMD8    (CMD0 + 8)
#define CMD9    (CMD0 + 9)
#define CMD10   (CMD0 + 10)
#define CMD12   (CMD0 + 12)
#define CMD13   (CMD0 + 13)
#define CMD16   (CMD0 + 16)
#define CMD17   (CMD0 + 17)
#define CMD18   (CMD0 + 18)
#define CMD24   (CMD0 + 24)
#define CMD25   (CMD0 + 25)
#define CMD27   (CMD0 + 27)
#define CMD28   (CMD0 + 28)
#define CMD29   (CMD0 + 29)
#define CMD30   (CMD0 + 30)
#define CMD32   (CMD0 + 32)
#define CMD33   (CMD0 + 33)
#define CMD38   (CMD0 + 38)
#define CMD42   (CMD0 + 42)
#define CMD55   (CMD0 + 55)
#define CMD56   (CMD0 + 56)
#define CMD58   (CMD0 + 58)
#define CMD59   (CMD0 + 59)



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