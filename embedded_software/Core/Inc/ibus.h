#ifndef __IBUS_H__
#define __IBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define IBUS_FRAME_SIZE 32
#define IBUS_CHANNEL_COUNT 10
#define IBUS_FRAME_HEADER1 0x20
#define IBUS_FRAME_HEADER2 0x40
#define IBUS_DMA_BUFFER_SIZE 64

typedef struct {
    uint16_t left_horizontal, left_vertical;
    uint16_t right_horizontal, right_vertical;
    uint16_t pot1, pot2;
    uint16_t switch1, switch2, switch3, switch4;
}iBus_Data_t;

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t buffer[IBUS_DMA_BUFFER_SIZE];
    uint8_t frame[IBUS_FRAME_SIZE];
    uint8_t frameReady;
    uint8_t dataValid;
    uint16_t dmaLastPos;
    uint8_t isInitialized;
    iBus_Data_t *data;
}IBUS_Handle_t;

HAL_StatusTypeDef IBUS_Init(IBUS_Handle_t *ibus, UART_HandleTypeDef *huart, iBus_Data_t *data);
HAL_StatusTypeDef IBUS_ParseFrame(IBUS_Handle_t *ibus);
uint8_t IBUS_IsFrameValid(IBUS_Handle_t *ibus);
void IBUS_IdleLineCallback(IBUS_Handle_t *ibus);

#ifdef __cplusplus
}
#endif
#endif