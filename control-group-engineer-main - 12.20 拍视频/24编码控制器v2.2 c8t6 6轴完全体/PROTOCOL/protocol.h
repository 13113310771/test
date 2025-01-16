#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stm32f10x.h"





/**
  * @brief  帧头结构定义
  */
typedef __packed struct
{
  uint8_t  sof;
  uint16_t data_length;
  uint8_t  seq;
  uint8_t  crc8;
} frame_header_t;


void append_crc8_check_sum(uint8_t* pchMessage, uint16_t dwLength);
void append_crc16_check_sum(uint8_t* pchMessage, uint32_t dwLength);

#endif



