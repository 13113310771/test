#include "as5600_task.h"
#include "delay.h"

uint8_t Recive_Angle[AS5600_NUMBER*2],AS5600_Angle[AS5600_NUMBER*2],
				
				Ack_Test[4]={1,1,1,1},Data_Test[18];
				
uint16_t AS5600_Angle_16bit[AS5600_NUMBER],
					Data_Test_16bit[9];
				
int adress;
AMS5600Registers_t AMS5600Registers,AMS5600Registers_Read;

void as5600_Init_task(void)
{
		//开始指定地址
	IIC_START();
	
	IIC_SendAdress(_ams5600_Address,IIC_Send_Mode);//指名as5600
	IIC_ReciveAck();
	
	IIC_SendByte(_conf_hi);
	IIC_ReciveAck();
	IIC_SendByte(CONF_HI_PARAMETER);
	IIC_ReciveAck();
	
	IIC_STOP();
	
			//开始指定地址
	IIC_START();
	
	IIC_SendAdress(_ams5600_Address,IIC_Send_Mode);//指名as5600
	IIC_ReciveAck();
	
	IIC_SendByte(_conf_lo);
	IIC_ReciveAck();
	IIC_SendByte(CONF_LO_PARAMETER);
	IIC_ReciveAck();
	
	IIC_STOP();
	
}

void as5600_Test_task(void)
{
	int i;
	for(i=0;i<=0xff;i++)
	{
		//开始指定地址
		IIC_START();
		
	//	IIC_SendAdress(_ams5600_Address,IIC_Send_Mode);//指名as5600
		IIC_SendAdress(i,IIC_Send_Mode);//指名as5600
		
		Ack_Test[1]=IIC_ReciveAck();
		
		if(Ack_Test[1]==0 && Ack_Test[2]==0) 
		{
			Ack_Test[3]=0;
			adress=i;
		}
		if(Ack_Test[1]==0) {Ack_Test[2]=0;}

		IIC_STOP();
		delay_ms(40);
	}
}

void AMS5600Registers_Read_task(void)
{
	int i,adress=0;
	for(i=0;i<=255;i++)
	{
				
		if(
			i==   _zmco || 
			i== _zpos_hi || 
			i== _zpos_lo || 
			i== _mpos_hi || 
			i== _mpos_lo || 
			i== _mang_hi || 
			i== _mang_lo || 
			i== _conf_hi ||     
			i== _conf_lo || 
			i== _raw_ang_hi || 
			i== _raw_ang_lo || 
			i== _ang_hi || 
			i== _ang_lo || 
			i== _stat || 
			i== _agc || 
			i== _mag_hi || 
			i== _mag_lo || 
			i== _burn  
		)
		{
			//开始指定地址
			IIC_START();
			
			IIC_SendAdress(_ams5600_Address,IIC_Send_Mode);//指名as5600
			Ack_Test[0]=IIC_ReciveAck();
		
			IIC_SendByte(i);
			Ack_Test[1]=IIC_ReciveAck();
			
			//开始读取
			IIC_START();
			
			IIC_SendAdress(_ams5600_Address,IIC_Receive_Mode);
			Ack_Test[2]=IIC_ReciveAck();
			
			*( &AMS5600Registers_Read._zmco + adress )=IIC_ReciveByte();
			IIC_SendAck(1);//应答0
			
			IIC_STOP();
			adress++;
		}
	}
}

void as5600_task(void)
{
	uint16_t i;
	uint8_t Ack_Fill[AS5600_NUMBER],Ack_Depands[AS5600_NUMBER],Data_8bit_1[AS5600_NUMBER],Data_8bit_2[AS5600_NUMBER];
	//开始指定地址
	IIC_START();
	
	IIC_SendAdress(_ams5600_Address,IIC_Send_Mode);//指名as5600
	
	More_IIC_ReciveAck(Ack_Fill);
	
	IIC_SendByte(_ang_hi);
	More_IIC_ReciveAck(Ack_Fill);
	

	//开始读取
	IIC_START();
	
	IIC_SendAdress(_ams5600_Address,IIC_Receive_Mode);
	More_IIC_ReciveAck(Ack_Depands);

	More_IIC_ReciveByte(Data_8bit_1);//8位一维数组，每位存储对应号位AS5600
	IIC_SendAck(0);//应答0
	
	More_IIC_ReciveByte(Data_8bit_2);
	IIC_SendAck(1);//非应答
	
	
	for(i=0;i<AS5600_NUMBER;i++)
	{
		if(Ack_Depands[i]==0)
		{
			AS5600_Angle[2*i]=Data_8bit_1[i];
			AS5600_Angle[2*i+1]=Data_8bit_2[i];
			AS5600_Angle_16bit[i]=(AS5600_Angle[2*i]<<8 | AS5600_Angle[2*i+1]);
		}
	}
	IIC_STOP();
}






