#ifndef _user_H
#define _user_H

#define EXCHANGE_OFF   	 	 GPIO_SetBits(GPIOE,GPIO_Pin_6);
#define EXCHANGE_ON   	 		GPIO_ResetBits(GPIOE,GPIO_Pin_6);

#define CLAMP_OFF   	 	   GPIO_SetBits(GPIOC,GPIO_Pin_2);
#define CLAMP_ON   	 				GPIO_ResetBits(GPIOC,GPIO_Pin_2);

#define STORE_OFF   	 	   GPIO_SetBits(GPIOE,GPIO_Pin_4);
#define STORE_ON   	 			GPIO_ResetBits(GPIOE,GPIO_Pin_4);


#define PUMP_ON   	 	 {CLAMP_ON  EXCHANGE_ON}
#define PUMP_OFF   	 		{CLAMP_OFF  EXCHANGE_OFF}

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
	SPEED_MODE=1,
	ANGLE_MODE,
}PID_Mode_t;




#endif
