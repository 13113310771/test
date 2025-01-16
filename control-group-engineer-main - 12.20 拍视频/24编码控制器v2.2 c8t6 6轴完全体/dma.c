#include "dma.h"

/*USART3*/
uint8_t  dbus_buf[2][DBUS_MAX_LEN];
/*USART2*/
uint8_t  usart2_TXbuf[USART2_BUFLEN];
uint8_t  usart2_RXbuf[USART2_BUFLEN];
/*USART6*/
uint8_t  judge_rxbuf[2][JUDGE_MAX_LEN];
/*USART1*/
uint8_t  pc_rxbuf[2][PC_MAX_LEN];
/*SPI5*/
uint8_t  spi1_rx_buf[SPI1_BUFLEN];
uint8_t  spi1_tx_buf[SPI1_BUFLEN];

void USART3_DMA(void)
{
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
	
	DMA_InitTypeDef DMA_InitStructure;									//???????
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);;						//?????,????AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//??????,????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//??????,????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)dbus_buf[0];							//??????,????AddrB
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//???????,????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//???????,????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//??????,?????????
	DMA_InitStructure.DMA_BufferSize = DBUS_MAX_LEN;								//???????(????)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//??,??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;								//???????,????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//???,????
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);							//????????DMA_Init,??DMA1???1

	DMA_Cmd(DMA1_Channel1, DISABLE);
	DMA_Cmd(DMA1_Channel1,ENABLE);
}


void USART2_DMA(void)
{
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
	DMA_InitTypeDef DMA_InitStructure;								//???????
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);;						//?????,????AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//??????,????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//??????,????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)judge_rxbuf[0];						//??????,????AddrB
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//???????,????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//???????,????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//??????,?????????
	DMA_InitStructure.DMA_BufferSize =DMA_MemoryDataSize_Byte;								//???????(????)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//??,??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;								//???????,????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//???,????
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);							//????????DMA_Init,??DMA1???1
	
	/*DMA??*/
	DMA_Cmd(DMA1_Channel2, ENABLE);	//???????,??????????,?????Transfer?,???
	DMA_Cmd(DMA1_Channel2, DISABLE);
}

void USART1_DMA(void)
{
	
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);						//??DMA???
	
	/*DMA???*/
	DMA_InitTypeDef DMA_InitStructure;									//???????
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);;						//?????,????AddrA
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//??????,????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//??????,????
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pc_rxbuf[0];						//??????,????AddrB
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//???????,????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//???????,????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//??????,?????????
	DMA_InitStructure.DMA_BufferSize =DMA_MemoryDataSize_Byte;								//???????(????)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//??,??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;								//???????,????
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;					//???,????
	DMA_Init(DMA2_Channel1   , &DMA_InitStructure);							//????????DMA_Init,??DMA1???1
	
	/*DMA??*/
	DMA_Cmd(DMA2_Channel1   , DISABLE);	//???????,??????????,?????Transfer?,???
	
}

////GPT��������
//void USART1_DMA(void)
//{
//    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);   // ʹ�� DMA1 ʱ��
//    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);      // ʹ�� USART1 �� DMA ��������

//    DMA_InitTypeDef DMA_InitStructure;                  // DMA ���ýṹ��
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);  // �������ַΪ USART1 ���ݼĴ���
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݴ�СΪ�ֽ�
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // �����ַ������
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pc_rxbuf; // �洢������ַΪ pc_rxbuf ����
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // �洢�����ݴ�СΪ�ֽ�
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �洢����ַ����
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // ���ݴ��䷽��Ϊ�����赽�洢��
//    DMA_InitStructure.DMA_BufferSize = DMA_MemoryDataSize_Byte; // ���ݴ����СΪ BUFFER_SIZE
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // DMA ģʽΪ��ͨģʽ
//    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // DMA ���ȼ�Ϊ�е����ȼ�
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // ��ֹ�洢�����洢������
//    DMA_Init(DMA1_Channel5, &DMA_InitStructure); // ���� DMA1_Channel5

//    DMA_Cmd(DMA1_Channel5, ENABLE); // ʹ�� DMA1_Channel5
//}
/*SPI1*/
void SPI1_DMA(void)
{

//    NVIC_InitTypeDef NVIC_InitStructure;
//    DMA_InitTypeDef DMA_InitStructure;
//    /* -------------- Enable Module Clock Source ----------------------------*/
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
//		SPI_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
//		SPI_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);

////    DMA_DeInit(DMA2_Stream5);
////		DMA_DeInit(DMA2_Stream4);
//	  DMA_DeInit(DMA2_Stream3);
//		DMA_DeInit(DMA2_Stream2);
//	   /* SPI1_RX Init */
//    while (DMA_GetCmdStatus(DMA2_Stream2) != DISABLE)
//    {
//        ;
//    }
//    DMA_InitStructure.DMA_Channel = DMA_Channel_3;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->DR);
//			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)spi1_rx_buf;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
//			DMA_InitStructure.DMA_BufferSize = SPI1_BUFLEN;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//		  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//		DMA_Init(DMA2_Stream2, &DMA_InitStructure);
//    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
//    /* SPI1_TX Init */
//    while (DMA_GetCmdStatus(DMA2_Stream3) != DISABLE)
//    {
//        ;
//    }
//    DMA_InitStructure.DMA_Channel = DMA_Channel_3;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->DR);
//			DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)spi1_tx_buf;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//			DMA_InitStructure.DMA_BufferSize = SPI1_BUFLEN;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//			DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;	
//		DMA_Init(DMA2_Stream3, &DMA_InitStructure);
//		DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE);

//		DMA_Cmd(DMA2_Stream3, DISABLE); //Add a disable
//    DMA_Cmd(DMA2_Stream2, DISABLE); //Add a disable
//		DMA_Cmd(DMA2_Stream3, ENABLE); //Add a enable
//    DMA_Cmd(DMA2_Stream2, ENABLE); //Add a enable
//		
//		NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream2_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//		
//
}
