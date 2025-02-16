#ifndef _transmit_task_H
#define _transmit_task_H

#include "stm32f10x.h"

#include "data_packet.h"
#include "protocol.h"
#include "string.h"
#include "as5600_task.h"

#define PROTOCAL_FRAME_MAX_SIZE  200
#define DN_REG_ID    0xA5  //裁判系统帧头
#define HEADER_LEN   sizeof(frame_header_t)
#define CMD_LEN      2    //cmdid bytes
#define CRC_LEN      2    //crc16 bytes
#define DATA_ID			 0x0302//数据类型（自定义控制器）
#define CONTROLLER_DATA_LENGTH 12
#define JUDGE_FORMAT_LENGTH 9

#define MODEL_CMDMAIN_DATA_LEN sizeof(model_data)+1+17

/******发送的传感器数据类型********/
#define MODEL_MODE 0
#define GYROSOPE_MODE 1
#define CONTROLLER_MODE MODEL_MODE

#define CONTROLLER_ALL_DATA_LENGTH CONTROLLER_DATA_LENGTH+JUDGE_FORMAT_LENGTH

typedef struct
{
	uint16_t x;
	uint16_t y;
	uint16_t z;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t roll;
}gyroscope_data_t;

typedef struct
{
	uint8_t cmd_id;
	uint8_t main_data[29];
}usart_data_t;

//typedef struct
//{
//	uint16_t a;
//	uint16_t b;
//	uint16_t c;
//	uint16_t d;
//	uint16_t e;
//	uint16_t f;
//}model_data_t;

void transmit_task(void);
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t data);
void Usart_SendByte_2( USART_TypeDef * pUSARTx, uint16_t data);
void Usart_SendByte_pro( USART_TypeDef * pUSARTx, uint8_t *p_data,uint16_t length);
void Usart_SendStrin( USART_TypeDef * pUSARTx, char *str);

extern gyroscope_data_t gyroscope_data;
extern int16_t model_data[AS5600_NUMBER];
extern uint8_t data[CONTROLLER_DATA_LENGTH];
#endif
