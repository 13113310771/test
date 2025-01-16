#include "transmit_task.h"
#include "data_packet.h"
#include "delay.h"

#include "conformity_task.h"

gyroscope_data_t gyroscope_data;
usart_data_t usart_data;

uint8_t usart_data_pack[50];//数据包 <--优化

uint8_t data[CONTROLLER_DATA_LENGTH],data_output[CONTROLLER_ALL_DATA_LENGTH],
				data_test_1[CONTROLLER_DATA_LENGTH],data_test_2[CONTROLLER_DATA_LENGTH];
//void transmit_test(uint16_t model_data[6])
//{
//	memcpy(data,&gyroscope_data,CONTROLLER_DATA_LENGTH);
//}
//main
void transmit_task()
{
	usart_data.cmd_id=controller_mode;
	//数据复制
	switch(controller_mode)
	{
		case MODEL_MODE://加入模式放入发送数据中
		{
			memcpy(usart_data.main_data,model_data,sizeof(model_data));
			data_packet_pack(DATA_ID,(uint8_t *)&usart_data,usart_data_pack,MODEL_CMDMAIN_DATA_LEN,DN_REG_ID);//打包数据  <--优化
			Usart_SendByte_pro(USART1,usart_data_pack,MODEL_CMDMAIN_DATA_LEN+JUDGE_FORMAT_LENGTH);//发送函数
		}break;
		
		
		case GYROSOPE_MODE:
		memcpy(data,model_data,CONTROLLER_DATA_LENGTH);
		break;
	}
//	data_packet_pack(DATA_ID,data,data_output,CONTROLLER_DATA_LENGTH,DN_REG_ID);
	
//	Usart_SendByte_pro(USART1,data_output,CONTROLLER_ALL_DATA_LENGTH);//打包数据发送函数
	
	
}





void delay(int count)
{
	int i;
	for(i=1;i<=count;i++)
	;
}

frame_header_t frame_header;
//gyroscope_data_t main_data;
//controller_data_t controller_data;

/***************** 发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t data)
{
/* 发送一个字节数据到 USART */
	USART_SendData(pUSARTx,data);
/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
 
/***************** 发送两个字节 **********************/
void Usart_SendByte_2( USART_TypeDef * pUSARTx, uint16_t data)
{
	uint8_t data_h,data_l;
	data_h=(data&0xff00)>>8;
	data_l=data&0xff;

	USART_SendData(pUSARTx,data_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(pUSARTx,data_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}
/***************** 发送length个字节 **********************/
void Usart_SendByte_pro( USART_TypeDef * pUSARTx, uint8_t *p_data,uint16_t length)
{
	uint8_t data;
  uint16_t i;
	for(i=0;i<length;i++)
	{
		data=(*(p_data+i));
//		delay(1000);
		USART_SendData(pUSARTx,data);
		
		while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		
//		delay_ms(40);
	}
	
}
 /***************** 发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
	do {Usart_SendByte( pUSARTx, *(str + k) );k++;} while (*(str + k)!='\0');/* 等待发送完成 */
	while (USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET) {}
}

/***************** 发送遥控器数据 **********************/
void send_controller_data()
{
	Usart_SendByte(USART2,0xA5);//+*
}
