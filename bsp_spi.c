#include "bsp_spi.h"
#include "bsp_dwt.h"


void MySPI_W_SS_M1(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)BitValue);		//����BitValue������SS���ŵĵ�ƽ
}

void MySPI_W_SS_M2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)BitValue);		//����BitValue������SS���ŵĵ�ƽ
}
 
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

void SPI_MT6816_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);
 
  
  /*!< ��������: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	/*!< ��������: MISO -- 	SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  

	/*!< ���� SPI_FLASH_SPI ����: CS_M1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*!< ���� SPI_FLASH_SPI ����: CS_M2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*����Ĭ�ϵ�ƽ*/
	MySPI_W_SS_M1(1);											//SSĬ�ϸߵ�ƽ
	SCK_H;
}
 
/**
  * ��    ����SPI��ʼ
  * ��    ������
  * �� �� ֵ����
  */
void MySPI_Start(uint8_t m)
{
	if(m == 1)
		MySPI_W_SS_M1(0);				//����SS����ʼʱ��
	if(m == 2)
		MySPI_W_SS_M2(0);				//����SS����ʼʱ��
	Delay(0x01);
	SCK_L;
}
 
/**
  * ��    ����SPI��ֹ
  * ��    ������
  * �� �� ֵ����
  */
void MySPI_Stop(uint8_t m)
{
	SCK_H; 
	Delay(0x01);
	if(m == 1)
		MySPI_W_SS_M1(1);				//����SS����ֹʱ��
	if(m == 2)
		MySPI_W_SS_M2(1);				//����SS����ֹʱ��
}
 
/**
  * ��    ����SPI��������һ���ֽڣ�ʹ��SPIģʽ3
  * ��    ����ByteSend Ҫ���͵�һ���ֽ�
  * �� �� ֵ�����յ�һ���ֽ�
  */
uint8_t MySPI_SwapByte(uint16_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);	//�ȴ��������ݼĴ�����
	
	SPI_I2S_SendData(SPI1, ByteSend);								//д�����ݵ��������ݼĴ�������ʼ����ʱ��
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);	//�ȴ��������ݼĴ����ǿ�
	
	return SPI_I2S_ReceiveData(SPI1);								//��ȡ���յ������ݲ�����
}


void mySPI_WriteByte(u8 TxData)                                        
{		
	SDA_OUT();
	u16 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++) 
	{
		//��������
	if(TxData & 0x80) 
		SDA_H; 		        
	else 
		SDA_L;
	
	TxData = (TxData << 1);
	//����ʱ���ߣ��ñ�������������		
	SCK_H; 
	Delay(0x01);
	SCK_L; 
	Delay(0x01);           		 
	}	
}

u8 mySPI_ReadByte(void)                                        
{		
	SDA_IN();
	uint8_t data;
	u16 bit_ctr;
  for(bit_ctr=0;bit_ctr<8;bit_ctr++) 
	{         
		SCK_H; 
		Delay(0x01);
		if(READ_SDA)   
			data |= 0x01; 
		else
			data |= 0x00;
		SCK_L; 
		Delay(0x01);
		data = (data<<1);	
	}
	return(data);           		  		    
}



float MT6816_Get_AngleData(u8 m)
{
	uint8_t DataH = 0x00;
	uint8_t DataL = 0x00;
	MySPI_Start(m);
  mySPI_WriteByte(0x83);            //��MT6816���Ͷ�ȡ�ǶȼĴ���ָ��
	DataH = mySPI_ReadByte();    //���Ƕ����ݶ�����
  MySPI_Stop(m);
	
	Delay(0x01);
	
  MySPI_Start(m);
  mySPI_WriteByte(0x84);
	DataL = mySPI_ReadByte();
	DataL &= 0XFC;
  MySPI_Stop(m);
	
	float MyAngle =  (((float)((DataH<<8|DataL)>>2))*360.0f/16384.0f);   //��0X03��0X04���ݽ���ƴ�Ӳ�����������λ������14λ�Ƕ����ݣ����ݹ�ʽ����ȡ��������ת��Ϊ�Ƕ�
 
	return MyAngle;
 
}


   
/*********************************************END OF FILE**********************/
