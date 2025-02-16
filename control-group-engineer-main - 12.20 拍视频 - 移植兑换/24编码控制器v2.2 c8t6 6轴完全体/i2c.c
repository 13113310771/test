#include "i2c.h"
#include "delay.h"

#include "as5600_task.h"

int test_data_number=0;
int test_gpio=3;
int test_gpio_out=0;
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_SCLK_PIN|IIC1_DATA_PIN|IIC2_DATA_PIN|IIC3_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC123_PORT,&GPIO_InitStructure);
	GPIO_SetBits(IIC123_PORT,IIC_SCLK_PIN|IIC1_DATA_PIN|IIC2_DATA_PIN|IIC3_DATA_PIN);//初始状态配置为浮空高电平
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC4_DATA_PIN|IIC5_DATA_PIN|IIC6_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC456_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(IIC456_PORT,IIC4_DATA_PIN|IIC5_DATA_PIN|IIC6_DATA_PIN);//初始状态配置为浮空高电平
}

void IIC_SCLK_Write(uint8_t x)
{
	GPIO_WriteBit(IIC123_PORT,IIC_SCLK_PIN,(BitAction)x);
	delay_us(10);
}

void IIC_DATA_Write(uint8_t x)
{
	GPIO_WriteBit(IIC123_PORT,IIC1_DATA_PIN,(BitAction)x);
	GPIO_WriteBit(IIC123_PORT,IIC2_DATA_PIN,(BitAction)x);
	GPIO_WriteBit(IIC123_PORT,IIC3_DATA_PIN,(BitAction)x);
	GPIO_WriteBit(IIC456_PORT,IIC4_DATA_PIN,(BitAction)x);
	GPIO_WriteBit(IIC456_PORT,IIC5_DATA_PIN,(BitAction)x);
	GPIO_WriteBit(IIC456_PORT,IIC6_DATA_PIN,(BitAction)x);
	delay_us(10);

}

uint8_t IIC_DATA_Read(void)
{
	uint8_t BitValue;
	
	BitValue=GPIO_ReadInputDataBit(IIC123_PORT,IIC1_DATA_PIN);
	return BitValue;
}

//uint8_t BitValue[10];
//uint8_t* IIC_DATA_Read(uint8_t* BitValue)
//{

//	BitValue[0]=GPIO_ReadInputDataBit(IIC123_PORT,IIC1_DATA_PIN);
//	delay_us(10);//其实应该不需要延时
//	return BitValue;
//}


void IIC_START(void)
{
	IIC_DATA_Write(1);//兼容重复启动
	IIC_SCLK_Write(1);
	
	//启动
	IIC_DATA_Write(0);
	IIC_SCLK_Write(0);

}

void IIC_STOP(void)
{
//	IIC_SCLK_Write(0);
	IIC_DATA_Write(0);
	
	//停止
	IIC_SCLK_Write(1);
	IIC_DATA_Write(1);
}


void IIC_SendByte(uint8_t byte)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		IIC_DATA_Write( byte & (128 >> i ) );//i在第一次进入时是0
		IIC_SCLK_Write(1);//从机读取
		IIC_SCLK_Write(0);//复位
	}
}

void IIC_SendAdress(uint8_t byte,uint8_t mode_flag)
{
	uint8_t i=0;
	for(i=1;i<8;i++)
	{
		IIC_DATA_Write( byte & (128 >> i ) );//i在第一次进入时是1,8位转换7位
		IIC_SCLK_Write(1);//从机读取
		IIC_SCLK_Write(0);//复位
	}
	IIC_DATA_Write(mode_flag);//i在第一次进入时是1,8位转换7位
	IIC_SCLK_Write(1);//从机读取
	IIC_SCLK_Write(0);//复位
}


uint8_t IIC_ReciveByte(void)
{
	uint8_t i,byte=0x00;
	IIC_DATA_Write(1);//主机放手
	for(i=0;i<8;i++)
	{
		IIC_SCLK_Write(1);//锁数据线
		if (IIC_DATA_Read()==1) byte |= (128>>i);//读取
		IIC_SCLK_Write(0);//复位
	}
	return byte;
}

void IIC_SendAck(uint8_t Ackbit)//发送应答  接收完后用
{

		IIC_DATA_Write(Ackbit);//改变
		IIC_SCLK_Write(1);//从机读取
		IIC_SCLK_Write(0);//复位
}

uint8_t IIC_ReciveAck(void)//接收应答  发送完后用
{
	uint8_t Ackbit=0x01;
	IIC_DATA_Write(1);//主机放手

	IIC_SCLK_Write(1);//锁数据线
	Ackbit=IIC_DATA_Read();//读取
	IIC_SCLK_Write(0);//复位
	return Ackbit;
}
void IIC_Test()
{
	IIC_DATA_Write(test_gpio_out);
	test_gpio=IIC_DATA_Read();

}




uint8_t More_BitValue[AS5600_NUMBER];
uint8_t* More_IIC_DATA_Read(uint8_t* More_BitValue)
{

	More_BitValue[0]=GPIO_ReadInputDataBit(IIC123_PORT,IIC1_DATA_PIN);
	More_BitValue[1]=GPIO_ReadInputDataBit(IIC123_PORT,IIC2_DATA_PIN);
	More_BitValue[2]=GPIO_ReadInputDataBit(IIC123_PORT,IIC3_DATA_PIN);
	More_BitValue[3]=GPIO_ReadInputDataBit(IIC456_PORT,IIC4_DATA_PIN);
	More_BitValue[4]=GPIO_ReadInputDataBit(IIC456_PORT,IIC5_DATA_PIN);
	More_BitValue[5]=GPIO_ReadInputDataBit(IIC456_PORT,IIC6_DATA_PIN);
	return More_BitValue;
}

void More_IIC_ReciveByte(uint8_t* More_byte)//传入一维8位数组，每位代表对应号传感器数据
{
	uint8_t i,j;//i表示位数，j表示操作的是第几个编码器
//	for(j=0;j<AS5600_NUMBER;j++)More_byte[j]=0;//重置
	
	IIC_DATA_Write(1);//主机放手
	for(i=0;i<8;i++)//逐字读取
	{
		IIC_SCLK_Write(1);//锁数据线
		More_IIC_DATA_Read(More_BitValue);
		for(j=0;j<AS5600_NUMBER;j++)//多编码器同位读取
		{
			More_byte[j] &= ~(128>>i);//重置  ~表示按位取反
			if (More_BitValue[j]==1) More_byte[j] |= (128>>i);//读取
		}
		IIC_SCLK_Write(0);//复位
	}
}

void More_IIC_ReciveAck(uint8_t*More_Ackbit)//接收应答  发送完后用
{
	uint16_t i;
	IIC_DATA_Write(1);//主机放手

	IIC_SCLK_Write(1);//锁数据线
	for(i=0;i<AS5600_NUMBER;i++)
	More_IIC_DATA_Read(More_Ackbit);//读取
	IIC_SCLK_Write(0);//复位
}
