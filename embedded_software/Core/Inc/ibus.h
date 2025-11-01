#ifndef __IBUS_H__
#define __IBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <string.h>

#define IBUS_SIZE 32
#define IBUS_CHANNEL_COUNT 10
#define IBUS_HEADER1 0x20
#define IBUS_HEADER2 0x40

typedef struct {
    uint16_t left_horizontal, left_vertical;
    uint16_t right_horizontal, right_vertical;
    uint16_t pot1, pot2;
    uint16_t switch1, switch2, switch3, switch4;
}iBus_Data_t;

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t buffer[IBUS_SIZE];
    uint8_t ready;
    uint8_t valid;
    iBus_Data_t *data;
}IBUS_Handle_t;

HAL_StatusTypeDef IBUS_Init(IBUS_Handle_t *ibus, UART_HandleTypeDef *huart, iBus_Data_t *data);
HAL_StatusTypeDef IBUS_Parse(IBUS_Handle_t *ibus);
uint8_t IBUS_IsValid(IBUS_Handle_t *ibus);

#ifdef __cplusplus
}
#endif
#endif
