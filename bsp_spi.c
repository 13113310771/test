#include "bsp_spi.h"
#include "bsp_dwt.h"


void MySPI_W_SS_M1(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, (BitAction)BitValue);		//根据BitValue，设置SS引脚的电平
}

void MySPI_W_SS_M2(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)BitValue);		//根据BitValue，设置SS引脚的电平
}
 
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}

void SPI_MT6816_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);
 
  
  /*!< 配置引脚: SCK */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	/*!< 配置引脚: MISO -- 	SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  

	/*!< 配置 SPI_FLASH_SPI 引脚: CS_M1 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*!< 配置 SPI_FLASH_SPI 引脚: CS_M2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*设置默认电平*/
	MySPI_W_SS_M1(1);											//SS默认高电平
	SCK_H;
}
 
/**
  * 函    数：SPI起始
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Start(uint8_t m)
{
	if(m == 1)
		MySPI_W_SS_M1(0);				//拉低SS，开始时序
	if(m == 2)
		MySPI_W_SS_M2(0);				//拉低SS，开始时序
	Delay(0x01);
	SCK_L;
}
 
/**
  * 函    数：SPI终止
  * 参    数：无
  * 返 回 值：无
  */
void MySPI_Stop(uint8_t m)
{
	SCK_H; 
	Delay(0x01);
	if(m == 1)
		MySPI_W_SS_M1(1);				//拉高SS，终止时序
	if(m == 2)
		MySPI_W_SS_M2(1);				//拉高SS，终止时序
}
 
/**
  * 函    数：SPI交换传输一个字节，使用SPI模式3
  * 参    数：ByteSend 要发送的一个字节
  * 返 回 值：接收的一个字节
  */
uint8_t MySPI_SwapByte(uint16_t ByteSend)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET);	//等待发送数据寄存器空
	
	SPI_I2S_SendData(SPI1, ByteSend);								//写入数据到发送数据寄存器，开始产生时序
	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) != SET);	//等待接收数据寄存器非空
	
	return SPI_I2S_ReceiveData(SPI1);								//读取接收到的数据并返回
}


void mySPI_WriteByte(u8 TxData)                                        
{		
	SDA_OUT();
	u16 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++) 
	{
		//建立数据
	if(TxData & 0x80) 
		SDA_H; 		        
	else 
		SDA_L;
	
	TxData = (TxData << 1);
	//操作时钟线，让编码器接收数据		
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
  mySPI_WriteByte(0x83);            //向MT6816发送读取角度寄存器指令
	DataH = mySPI_ReadByte();    //将角度数据读回来
  MySPI_Stop(m);
	
	Delay(0x01);
	
  MySPI_Start(m);
  mySPI_WriteByte(0x84);
	DataL = mySPI_ReadByte();
	DataL &= 0XFC;
  MySPI_Stop(m);
	
	float MyAngle =  (((float)((DataH<<8|DataL)>>2))*360.0f/16384.0f);   //将0X03和0X04数据进行拼接并将其右移两位，保留14位角度数据，根据公式将读取到的数据转换为角度
 
	return MyAngle;
 
}


   
/*********************************************END OF FILE**********************/
