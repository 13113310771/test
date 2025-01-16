#include "conformity_task.h"
#include "transmit_task.h"

int16_t model_data[AS5600_NUMBER],model_normal_angle[AS5600_NUMBER],model_angle[AS5600_NUMBER];
int32_t encoder_count_test_1=20;
int16_t encoder_count_test_2=20;
uint8_t controller_mode=MODEL_MODE;

void conformity_init()
{
  
	int i;
	for(i=0;i<AS5600_NUMBER;i++)model_data[i]=0;
	model_normal_angle[0]=-14;//front
	model_normal_angle[1]=147;//hight
	model_normal_angle[2]=-139;
	model_normal_angle[3]=-60;//yaw
	model_normal_angle[4]=40;//pitch
	model_normal_angle[5]=-142;//roll
}

int16_t model_last_angle[AS5600_NUMBER],count[AS5600_NUMBER]={0,0,0,0,0,0};
void model_task()
{
	uint8_t i;
	
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
	
	for(i=0;i<AS5600_NUMBER;i++)
	{
		model_data[i]=model_angle[i]-model_normal_angle[i];
	}
	
}



