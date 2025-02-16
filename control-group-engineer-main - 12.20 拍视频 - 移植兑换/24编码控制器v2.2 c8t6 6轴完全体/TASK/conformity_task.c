#include "conformity_task.h"
#include "transmit_task.h"

int16_t model_data[AS5600_NUMBER],model_normal_angle[AS5600_NUMBER];
int32_t encoder_count_test_1=20;
int16_t encoder_count_test_2=20;
uint8_t controller_mode=MODEL_MODE;

void conformity_init()
{
  
	int i;
	for(i=0;i<AS5600_NUMBER;i++)model_data[i]=0;
	model_normal_angle[0]=-15;//front
	model_normal_angle[1]=-149;//hight
	model_normal_angle[2]=-50;
	model_normal_angle[3]=0;//yaw
	model_normal_angle[4]=45;//pitch
	model_normal_angle[5]=-143;//roll
}

int16_t model_last_angle[AS5600_NUMBER],count[AS5600_NUMBER]={0,0,0,0,0,0};

void model_task()
{
	
	uint8_t i;
	int16_t model_angle[AS5600_NUMBER];
//	model_data[1]=(Encoder_TIM1_Get()*360/1500);
//	model_data[2]=Encoder_TIM8_Get();
	
	for(i=0;i<AS5600_NUMBER;i++)
	{
		model_angle[i]=AS5600_Angle_16bit[i]*360/4096;
		if(model_angle[i]-model_last_angle[i]<-180)
			count[i]++;
		if(model_angle[i]-model_last_angle[i]>180)
			count[i]--;
			model_last_angle[i]=model_angle[i];
		
		model_angle[i]=count[i]*360+model_angle[i];
		
	}
	
//	/*左边与工程上电机id对应，为使数据容量优化统一减以对应   右边从0到5依次为模型传感器由下而上计数*/
////	model_data[3 -1]=AS5600_Angle_16bit[0]*360/4096-model_normal_angle[0];
//	model_data[1-1]=-(model_angle[0]-model_normal_angle[1-1]);
//	model_data[2-1]=model_angle[1]-model_normal_angle[2-1]-model_data[1-1];
//	model_data[4-1]=-(model_angle[2]-model_normal_angle[4-1]);
//	model_data[5-1]=-(model_angle[4]-model_normal_angle[5-1]);
//	model_data[6-1]=-(model_angle[5]*666/90-model_normal_angle[6-1]);//舵机
	for(i=0;i<AS5600_NUMBER;i++)
	{
		model_data[i]=model_angle[i]-model_normal_angle[i];
	}
	
}



