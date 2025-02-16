#ifndef _data_packet_H
#define _data_packet_H

#include "stm32f10x.h"

#include "protocol.h"


extern uint16_t data_address;
void data_packet_pack(uint16_t cmd_id, uint8_t *p_data, uint8_t *data_output, uint16_t data_length, uint8_t sof);
#endif

