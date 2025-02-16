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
	GPIO_SetBits(IIC123_PORT,IIC_SCLK_PIN|IIC1_DATA_PIN|IIC2_DATA_PIN|IIC3_DATA_PIN);//��ʼ״̬����Ϊ���ոߵ�ƽ
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC4_DATA_PIN|IIC5_DATA_PIN|IIC6_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC456_PORT,&GPIO_InitStructure);
	
	GPIO_SetBits(IIC456_PORT,IIC4_DATA_PIN|IIC5_DATA_PIN|IIC6_DATA_PIN);//��ʼ״̬����Ϊ���ոߵ�ƽ
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
//	delay_us(10);//��ʵӦ�ò���Ҫ��ʱ
//	return BitValue;
//}


void IIC_START(void)
{
	IIC_DATA_Write(1);//�����ظ�����
	IIC_SCLK_Write(1);
	
	//����
	IIC_DATA_Write(0);
	IIC_SCLK_Write(0);

}

void IIC_STOP(void)
{
//	IIC_SCLK_Write(0);
	IIC_DATA_Write(0);
	
	//ֹͣ
	IIC_SCLK_Write(1);
	IIC_DATA_Write(1);
}


void IIC_SendByte(uint8_t byte)
{
	uint8_t i=0;
	for(i=0;i<8;i++)
	{
		IIC_DATA_Write( byte & (128 >> i ) );//i�ڵ�һ�ν���ʱ��0
		IIC_SCLK_Write(1);//�ӻ���ȡ
		IIC_SCLK_Write(0);//��λ
	}
}

void IIC_SendAdress(uint8_t byte,uint8_t mode_flag)
{
	uint8_t i=0;
	for(i=1;i<8;i++)
	{
		IIC_DATA_Write( byte & (128 >> i ) );//i�ڵ�һ�ν���ʱ��1,8λת��7λ
		IIC_SCLK_Write(1);//�ӻ���ȡ
		IIC_SCLK_Write(0);//��λ
	}
	IIC_DATA_Write(mode_flag);//i�ڵ�һ�ν���ʱ��1,8λת��7λ
	IIC_SCLK_Write(1);//�ӻ���ȡ
	IIC_SCLK_Write(0);//��λ
}


uint8_t IIC_ReciveByte(void)
{
	uint8_t i,byte=0x00;
	IIC_DATA_Write(1);//��������
	for(i=0;i<8;i++)
	{
		IIC_SCLK_Write(1);//��������
		if (IIC_DATA_Read()==1) byte |= (128>>i);//��ȡ
		IIC_SCLK_Write(0);//��λ
	}
	return byte;
}

void IIC_SendAck(uint8_t Ackbit)//����Ӧ��  ���������
{

		IIC_DATA_Write(Ackbit);//�ı�
		IIC_SCLK_Write(1);//�ӻ���ȡ
		IIC_SCLK_Write(0);//��λ
}

uint8_t IIC_ReciveAck(void)//����Ӧ��  ���������
{
	uint8_t Ackbit=0x01;
	IIC_DATA_Write(1);//��������

	IIC_SCLK_Write(1);//��������
	Ackbit=IIC_DATA_Read();//��ȡ
	IIC_SCLK_Write(0);//��λ
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

void More_IIC_ReciveByte(uint8_t* More_byte)//����һά8λ���飬ÿλ�����Ӧ�Ŵ���������
{
	uint8_t i,j;//i��ʾλ����j��ʾ�������ǵڼ���������
//	for(j=0;j<AS5600_NUMBER;j++)More_byte[j]=0;//����
	
	IIC_DATA_Write(1);//��������
	for(i=0;i<8;i++)//���ֶ�ȡ
	{
		IIC_SCLK_Write(1);//��������
		More_IIC_DATA_Read(More_BitValue);
		for(j=0;j<AS5600_NUMBER;j++)//�������ͬλ��ȡ
		{
			More_byte[j] &= ~(128>>i);//����  ~��ʾ��λȡ��
			if (More_BitValue[j]==1) More_byte[j] |= (128>>i);//��ȡ
		}
		IIC_SCLK_Write(0);//��λ
	}
}

void More_IIC_ReciveAck(uint8_t*More_Ackbit)//����Ӧ��  ���������
{
	uint16_t i;
	IIC_DATA_Write(1);//��������

	IIC_SCLK_Write(1);//��������
	for(i=0;i<AS5600_NUMBER;i++)
	More_IIC_DATA_Read(More_Ackbit);//��ȡ
	IIC_SCLK_Write(0);//��λ
}
