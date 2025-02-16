#ifndef _i2c_H
#define _i2c_H
#include "stm32f10x.h"                  // Device header




#define IIC123_PORT GPIOB
#define IIC_SCLK_PIN  GPIO_Pin_9
#define IIC1_DATA_PIN GPIO_Pin_8
#define IIC2_DATA_PIN GPIO_Pin_7
#define IIC3_DATA_PIN GPIO_Pin_6

#define IIC456_PORT GPIOB
#define IIC4_DATA_PIN GPIO_Pin_5
#define IIC5_DATA_PIN GPIO_Pin_4
#define IIC6_DATA_PIN GPIO_Pin_3

#define IIC_Send_Mode 0
#define IIC_Receive_Mode 1



extern uint8_t Ack_Test[4];

void IIC_Init(void);

void IIC_START(void);
void IIC_STOP(void);

void IIC_SendByte(uint8_t byte);
void IIC_SendAdress(uint8_t byte,uint8_t mode_flag);
uint8_t IIC_ReciveByte(void);

void IIC_SendAck(uint8_t Ackbit);//发送应答
uint8_t IIC_ReciveAck(void);//接收应答

void IIC_Test(void);

void More_IIC_ReciveByte(uint8_t* More_byte);
void More_IIC_ReciveAck(uint8_t*More_Ackbit);
#endif


