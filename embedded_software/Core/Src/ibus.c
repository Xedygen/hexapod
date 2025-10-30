#include "ibus.h"

// ibus packet is 32 byte size i think
// [0x20][0x40][CH1_L][CH1_H][CH2_L][CH2_H]...[CH14_L][CH14_H][Checksum_L][Checksum_H]
HAL_StatusTypeDef IBUS_Init(IBUS_Handle_t* ibus, UART_HandleTypeDef* huart,
                            iBus_Data_t* data) {
  memset(ibus, 0, sizeof(*ibus));
  ibus->huart = huart;
  ibus->data = data;
  ibus->frameReady = 0;
  ibus->dataValid = 0;
  ibus->dmaLastPos = 0;
  memset(ibus->data, 0, sizeof(iBus_Data_t));

  if (HAL_UARTEx_ReceiveToIdle_DMA(ibus->huart, ibus->buffer,
                                   IBUS_DMA_BUFFER_SIZE) != HAL_OK) {
    return HAL_ERROR;
  }
  __HAL_DMA_DISABLE_IT(huart->hdmarx, DMA_IT_HT);

  ibus->isInitialized = 1;
  return HAL_OK;
}

// sorry about this mess but i wrote it for my controllers channel configuration
// if something feels wrong you can just change the order of the data variables
//  you can check it from the menu on the controller before using it
HAL_StatusTypeDef IBUS_ParseFrame(IBUS_Handle_t* ibus) {
  if (ibus->frameReady == 0) {
    ibus->dataValid = 0;
    return HAL_ERROR;
  }
  ibus->frameReady = 0;

  uint8_t sync_offset = 0;

  for (sync_offset = 0; sync_offset < 4; sync_offset++) {
    if (ibus->frame[sync_offset] == IBUS_FRAME_HEADER1 &&
        ibus->frame[sync_offset + 1] == IBUS_FRAME_HEADER2) {
      if (sync_offset > 0) {
        memmove(ibus->frame, ibus->frame + sync_offset,
                IBUS_FRAME_SIZE - sync_offset);
        memset(ibus->frame + (IBUS_FRAME_SIZE - sync_offset), 0, sync_offset);

        ibus->dataValid = 0;
        return HAL_ERROR;
      }
      break;
    }
  }

  if (sync_offset >= 4) {
    ibus->dataValid = 0;
    return HAL_ERROR;
  }

  if (IBUS_IsFrameValid(ibus) != 1) {
    ibus->dataValid = 0;
    return HAL_ERROR;
  }
  ibus->data->right_horizontal = ibus->frame[3] << 8 | ibus->frame[2];  // ch1
  ibus->data->right_vertical = ibus->frame[5] << 8 | ibus->frame[4];    // ch2
  ibus->data->left_vertical = ibus->frame[7] << 8 | ibus->frame[6];     // ch3
  ibus->data->left_horizontal = ibus->frame[9] << 8 | ibus->frame[8];   // ch4
  ibus->data->pot1 = ibus->frame[11] << 8 | ibus->frame[10];            // ch5
  ibus->data->pot2 = ibus->frame[13] << 8 | ibus->frame[12];            // ch6
  ibus->data->switch1 = ibus->frame[15] << 8 | ibus->frame[14];         // ch7
  ibus->data->switch2 = ibus->frame[17] << 8 | ibus->frame[16];         // ch8
  ibus->data->switch3 = ibus->frame[19] << 8 | ibus->frame[18];         // ch9
  ibus->data->switch4 = ibus->frame[21] << 8 | ibus->frame[20];         // ch10

  ibus->dataValid = 1;
  return HAL_OK;
}

uint8_t IBUS_IsFrameValid(IBUS_Handle_t* ibus) {
  if (ibus->frame[0] != IBUS_FRAME_HEADER1 ||
      ibus->frame[1] != IBUS_FRAME_HEADER2) {
    return 0;
  }

  uint16_t checksum = 0;
  for (int i = 0; i < (IBUS_FRAME_SIZE - 2); i++) {
    checksum += ibus->frame[i];
  }

  if (0xFFFF - checksum != ((ibus->frame[IBUS_FRAME_SIZE - 1] << 8) |
                            (ibus->frame[IBUS_FRAME_SIZE - 2]))) {
    return 0;
  }

  return 1;
}

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
//   if (huart == ibus.huart) {
//     if (Size >= IBUS_FRAME_SIZE) {
//       memcpy(ibus.frame, &ibus.buffer[Size - IBUS_FRAME_SIZE], IBUS_FRAME_SIZE);
//       ibus.frameReady = 1;
//     }

//     HAL_UARTEx_ReceiveToIdle_DMA(huart, ibus.buffer, IBUS_DMA_BUFFER_SIZE);
//   }
// }