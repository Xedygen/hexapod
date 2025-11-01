#include "ibus.h"

// usage

// IBUS_Handle_t ibus;
// iBus_Data_t data;

// void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
// 	ibus.ready = 1;
// 	HAL_UARTEx_ReceiveToIdle_DMA(&huart4, ibus.buffer, IBUS_SIZE);
// }

// int main(void) {
//   IBUS_Init(&ibus, &huart4, &data);
//   while (1)
//   {
// 	  if (ibus.ready) {
// 		  IBUS_Parse(&ibus);
// 	  }
//   }
// }

// ibus packet is 32 byte size
// [0x20][0x40][CH1_L][CH1_H][CH2_L][CH2_H]...[CH14_L][CH14_H][Checksum_L][Checksum_H]
HAL_StatusTypeDef IBUS_Init(IBUS_Handle_t* ibus, UART_HandleTypeDef* huart, iBus_Data_t* data) {
  memset(ibus, 0, sizeof(*ibus));
  ibus->huart = huart;
  ibus->data = data;
  memset(ibus->data, 0, sizeof(iBus_Data_t));

  HAL_UARTEx_ReceiveToIdle_DMA(ibus->huart, ibus->buffer, IBUS_SIZE);

  return HAL_OK;
}

// sorry about this mess but i wrote it for my controllers channel configuration
// if something feels wrong you can just change the order of the data variables
//  you can check it from the menu on the controller before using it
HAL_StatusTypeDef IBUS_Parse(IBUS_Handle_t* ibus) {
  if (ibus->ready == 0) {
    ibus->valid = 0;
    return HAL_ERROR;
  }
  ibus->ready = 0;

  uint8_t sync_offset = 0;

  for (sync_offset = 0; sync_offset < 4; sync_offset++) {
    if (ibus->buffer[sync_offset] == IBUS_HEADER1 &&
        ibus->buffer[sync_offset + 1] == IBUS_HEADER2) {
      if (sync_offset > 0) {
        memmove(ibus->buffer, ibus->buffer + sync_offset,
                IBUS_SIZE - sync_offset);
        memset(ibus->buffer + (IBUS_SIZE - sync_offset), 0, sync_offset);

        ibus->valid = 0;
        return HAL_ERROR;
      }
      break;
    }
  }

  if (sync_offset >= 4) {
    ibus->valid = 0;
    return HAL_ERROR;
  }

  if (IBUS_IsValid(ibus) != 1) {
    ibus->valid = 0;
    return HAL_ERROR;
  }
  ibus->data->right_horizontal = ibus->buffer[3] << 8 | ibus->buffer[2];  // ch1
  ibus->data->right_vertical = ibus->buffer[5] << 8 | ibus->buffer[4];    // ch2
  ibus->data->left_vertical = ibus->buffer[7] << 8 | ibus->buffer[6];     // ch3
  ibus->data->left_horizontal = ibus->buffer[9] << 8 | ibus->buffer[8];   // ch4
  ibus->data->pot1 = ibus->buffer[11] << 8 | ibus->buffer[10];            // ch5
  ibus->data->pot2 = ibus->buffer[13] << 8 | ibus->buffer[12];            // ch6
  ibus->data->switch1 = ibus->buffer[15] << 8 | ibus->buffer[14];         // ch7
  ibus->data->switch2 = ibus->buffer[17] << 8 | ibus->buffer[16];         // ch8
  ibus->data->switch3 = ibus->buffer[19] << 8 | ibus->buffer[18];         // ch9
  ibus->data->switch4 = ibus->buffer[21] << 8 | ibus->buffer[20];         // ch10

  ibus->valid = 1;
  return HAL_OK;
}

uint8_t IBUS_IsValid(IBUS_Handle_t* ibus) {
  if (ibus->buffer[0] != IBUS_HEADER1 ||
      ibus->buffer[1] != IBUS_HEADER2) {
    return 0;
  }

  uint16_t checksum = 0;
  for (int i = 0; i < (IBUS_SIZE - 2); i++) {
    checksum += ibus->buffer[i];
  }

  if (0xFFFF - checksum != ((ibus->buffer[IBUS_SIZE - 1] << 8) | (ibus->buffer[IBUS_SIZE - 2]))) {
    return 0;
  }

  return 1;
}