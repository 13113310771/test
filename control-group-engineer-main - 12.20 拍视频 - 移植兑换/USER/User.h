#ifndef _user_H
#define _user_H
/*现在是吸气时要让电磁阀置低电平*/
#define EXCHANGE_Solenoid_OFF   	 	 GPIO_SetBits(GPIOE,GPIO_Pin_5);
#define EXCHANGE_Solenoid_ON   	 		GPIO_ResetBits(GPIOE,GPIO_Pin_5);

#define EXCHANGE_PUMP_OFF           GPIO_ResetBits(GPIOC,GPIO_Pin_2);
#define EXCHANGE_PUMP_ON           GPIO_SetBits(GPIOC,GPIO_Pin_2);

#define CLAMP_Solenoid_OFF   	 	    GPIO_SetBits(GPIOF,GPIO_Pin_0);
#define CLAMP_Solenoid_ON   	 			GPIO_ResetBits(GPIOF,GPIO_Pin_0);

#define CLAMP_PUMP_OFF   	 	    GPIO_ResetBits(GPIOE,GPIO_Pin_6);
#define CLAMP_PUMP_ON   	 			GPIO_SetBits(GPIOE,GPIO_Pin_6);


#define EXCHANGE_ON   	 	 {EXCHANGE_Solenoid_ON  EXCHANGE_PUMP_ON}
#define EXCHANGE_OFF			 {EXCHANGE_Solenoid_OFF  EXCHANGE_PUMP_ON}


#define CLAMP_ON					{CLAMP_Solenoid_ON  CLAMP_PUMP_ON}
#define CLAMP_OFF   	 		{CLAMP_Solenoid_OFF  CLAMP_PUMP_ON }

#define ALL_ON						{EXCHANGE_Solenoid_ON  EXCHANGE_PUMP_ON CLAMP_PUMP_ON CLAMP_Solenoid_ON}
#define ALL_OFF						{EXCHANGE_Solenoid_OFF  EXCHANGE_PUMP_OFF CLAMP_PUMP_OFF CLAMP_Solenoid_OFF}


//ɾ
#define MANIPULATOR_6020_ID 0
#define SLIDE_NUMBER 4

#define	ADRUST_X 0
#define	ADRUST_Y 1
#define	ADRUST_Z 2
#define	ADRUST_YAW 3
#define	ADRUST_PITCH 4
#define	ADRUST_ROLL 5
//#define BACK_DRIVE 

typedef enum
{
	SPEED_MODE=0,
	ANGLE_MODE,
}PID_Mode_t;




#endif
