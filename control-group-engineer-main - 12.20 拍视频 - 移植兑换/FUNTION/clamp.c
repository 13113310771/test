/*****************************************************
	  此文件用作motor_task的从属文件，用于存放夹取用的
堵转初始化与动作等函数以避免motor_task文件太长而翻找
麻烦
*****************************************************/
#include "STM32_TIM_BASE.h"

#include "clamp.h"
#include "motor_task.h"
#include "modeswitch_task.h"
#include "remote_ctrl.h"
#include "controller.h"
#include "keyboard.h"
int16_t test1=-2500;
int16_t test2=20;
int16_t test3=0;

int16_t smalltest1=-250;
int16_t smalltest2=180;
int16_t smalltest3=0;

Motor_angle_t Mode;

uint32_t clamp_record_time;
uint8_t action_one_state=ACTIONING;
uint8_t action_two_state=ACTIONING;
uint8_t action_three_state=ACTIONING;
uint8_t have_box_number=0;

uint8_t current_limit_intit_flag=0;
Motor_current_limit_t Motor_current_limit[MOTOR_NUMBER]={0};

uint32_t test_time=0;
uint32_t test_time_flag=0;

int16_t limit_angle=0;
void clamp_angle_handle()
{
	uint8_t ID;
	
	if(current_limit_intit_flag==0)
		current_limit_intit_flag=Motor_current_limit_init();
	Motor_current_limit_reset();
	
	switch(chassis_mode)
	{
		case CHASSIS_NORMAL_MODE:
		{
			normal_clmap_handler();
			clamp_action=CLAMP_UN_CMD;
		}break;
		case CHASSIS_CLAMP_BIG_ISLAND_STRAIGHT_MODE:
		{
			big_island_stright_clamp_handler();
		}break;
		case CHASSIS_CLAMP_BIG_ISLAND_SLANTED_MODE:
		{
			big_island_slanted_clamp_handler();
		}break;
		case CHASSIS_CLAMP_SMALL_ISLAND_MODE:
		{
			small_island_clamp_handler();
		}break;
		case CHASSIS_GROUND_MODE:
		{
			clamp_ground_handler();
		}break;
		case CHASSIS_EXCHANGE_MODE:
		case CHASSIS_ANGLE_MODE:
		{
			exchange_handler();
		}break;
		case CHASSIS_CHECK_MODE:
		{
			check_handler();
		}
		default:
		{
			clamp_action=CLAMP_UN_CMD;
			//ALL_OFF;
		}
		break;
	}
	
	Motor_current_limit_handler();
	
	for(ID=0;ID<MOTOR_NUMBER;ID++)//
	{
	if(Motor[ID].MOTOR_NAME>CHASSIS_BR && Whether_Brushless_Motor(Motor[ID]))//底盘没改，不在这里进行计算
		{
			Motor[ID].Brushless.angle_ref= Motor[ID].Angle.offset_angle + Motor[ID].Angle.normal_angle + Motor[ID].Angle.mode_angle;
		}
	else if(Whether_Brushless_Motor(Motor[ID])==0)
		{
			Motor[ID].Servo.angle_ref=Motor[ID].Angle.normal_angle + Motor[ID].Angle.mode_angle;
		}
	}
	
	Motor[SLIDE_L].Brushless.angle_ref-=(model_out_angle[0]+kb_adrust[ADRUST_X].angle);
	Motor[SLIDE_R].Brushless.angle_ref+=(model_out_angle[0]+kb_adrust[ADRUST_X].angle);
	
	Motor[UPRISE_L].Brushless.angle_ref+=(model_out_angle[2]+kb_adrust[ADRUST_Z].angle);
	Motor[UPRISE_R].Brushless.angle_ref+=(model_out_angle[2]+kb_adrust[ADRUST_Z].angle);
	
	Motor[EXCHANGE_YAW].Brushless.angle_ref+=(model_out_angle[3]-kb_adrust[ADRUST_YAW].angle);
	Motor[EXCHANGE_PITCH].Brushless.angle_ref-=(model_out_angle[4]+kb_adrust[ADRUST_PITCH].angle);
	Motor[EXCHANGE_ROLL].Servo.angle_ref-=(model_out_angle[5]+kb_adrust[ADRUST_ROLL].angle);
	
	Motor[SLIDE_CLAMP].Brushless.angle_ref+=kb_slide_clamp_del.angle;
	
	if(Motor[SLIDE_L].Brushless.angle_ref>Motor[SLIDE_L].Angle.offset_angle-SLIDE_MAX_CHANGE)
	{
		
		limit_angle=kb_adrust[ADRUST_X].angle;
	}
	/******************限制伸出*******************************/
	if(Motor[SLIDE_L].Brushless.angle_ref<Motor[SLIDE_L].Angle.offset_angle-SLIDE_MAX_CHANGE)
	{
		Motor[SLIDE_L].Brushless.angle_ref=Motor[SLIDE_L].Angle.offset_angle-SLIDE_MAX_CHANGE;
		kb_adrust[ADRUST_X].angle=limit_angle;
	}
		
	if(Motor[SLIDE_R].Brushless.angle_ref>Motor[SLIDE_R].Angle.offset_angle+SLIDE_MAX_CHANGE)
		Motor[SLIDE_R].Brushless.angle_ref=Motor[SLIDE_R].Angle.offset_angle+SLIDE_MAX_CHANGE;
}

void normal_clmap_handler(void)
{
	uint8_t ID;
	
	for(ID=UPRISE_L;ID<=MOTOR_MAX_ID;ID++)
		Motor[ID].Angle.mode_angle=0;
	
	//EXCHANGE_ON
	//CLAMP_ON
	
	EXCHANGE_PUMP_ON
	//EXCHANGE_Solenoid_OFF
	CLAMP_PUMP_ON
	//CLAMP_Solenoid_OFF
	
	
	if(have_box_number==0)
	{
		EXCHANGE_PUMP_OFF
		CLAMP_PUMP_OFF
	}
		
	else 
	{
		EXCHANGE_PUMP_ON
	  CLAMP_PUMP_ON
		EXCHANGE_Solenoid_ON
		
	}
		
}

void big_island_stright_clamp_handler(void)
{
	switch(clamp_action)
	{
		case CLAMP_UN_CMD:
		{
			Motor_change_mode_angle(&Mode.bigisland_straight_angle,UPRISE_L,MOTOR_MAX_ID);//只将夹取相关电机置对应模式角度
			clamp_record_time= HAL_GetTick();
			action_one_state=ACTIONING;
			action_two_state=ACTIONING;
			ALL_ON
		}break;
		case ACTION_ONE:
		{
			if(action_one_state==ACTIONING)
			{
					if((HAL_GetTick()-clamp_record_time)==200)
				{
					Motor_uprise_angle_change(7000/542*48*2);//1500
					Motor[CLAMP_PITCH].Angle.mode_angle-=10;
				}
				else if((HAL_GetTick()-clamp_record_time)>1500)
				{
					Motor[SLIDE_CLAMP].Angle.mode_angle+=800;
					if(have_box_number<MAX_BOX_NUMBER)
						have_box_number++;
					action_one_state=ACTION_DONE;
				}
			}
			else
//				Motor_current_limit[SLIDE_CLAMP].flag=1;//使能限流
				clamp_record_time= HAL_GetTick();
		}break;
		case ACTION_TWO:
		{
			if(action_two_state==ACTIONING)
				action_two_state=store_handler();
			else
				clamp_action=CLAMP_UN_CMD;
		}break;
	}
}

uint8_t store_big_island_slanted_second_box=0;
void big_island_slanted_clamp_handler(void)
{
  switch(clamp_action)
  {
    case CLAMP_UN_CMD:
    {
      Motor_change_mode_angle(&Mode.bigisland_slanted_angle,UPRISE_L,MOTOR_MAX_ID);
      clamp_record_time= HAL_GetTick();
      action_one_state=ACTIONING;
      action_two_state=ACTIONING;
			ALL_ON
    }break;
    case ACTION_ONE:
    {
			if(action_one_state==ACTIONING)
			{
				if((HAL_GetTick()-clamp_record_time)==200)
				{
					Motor_uprise_angle_change(7000/542*48*2);//1500
					Motor[CLAMP_PITCH].Angle.mode_angle-=20;
				}
				else if((HAL_GetTick()-clamp_record_time)>2000)
				{
//					Motor[SLIDE_CLAMP].Angle.mode_angle+=500;
					if(have_box_number<MAX_BOX_NUMBER)
						have_box_number++;
					action_one_state=ACTION_DONE;
				}
			}
			else
			{	
				Motor_current_limit[SLIDE_CLAMP].flag=1;//使能限流
        clamp_record_time= HAL_GetTick();
    
			}
			}break;
   case ACTION_TWO:
		{
			if(action_two_state==ACTIONING)
				action_two_state=store_handler();
			else
				clamp_action=CLAMP_UN_CMD;
		}break;
  }
}

void small_island_clamp_handler(void)
{
  switch(clamp_action)
  {
    case CLAMP_UN_CMD:
    {
      Motor_change_mode_angle(&Mode.ground_angle,UPRISE_L,MOTOR_MAX_ID);
      clamp_record_time= HAL_GetTick();
      action_one_state=ACTIONING;
      action_two_state=ACTIONING;
			ALL_ON
    }break;
    case ACTION_ONE:
    {
      if(action_one_state==ACTIONING)
      {
				if((HAL_GetTick()-clamp_record_time)==200)
				{
					Motor_uprise_angle_change(-1500);
				}
				if((HAL_GetTick()-clamp_record_time)==2000)
				{
					Motor_uprise_angle_change(7000);
				}
				else if((HAL_GetTick()-clamp_record_time)>=3500)
				{
					action_one_state=ACTION_DONE;
				}
      }
      else
        clamp_record_time= HAL_GetTick();
    }break;
    case ACTION_TWO:
    {
      if(action_two_state==ACTIONING)
        action_two_state=store_handler();
			else
				clamp_action=CLAMP_UN_CMD;
    }break;
  }
}

void clamp_ground_handler(void)
{
  switch(clamp_action)
  {
    case CLAMP_UN_CMD:
    {
      Motor_change_mode_angle(&Mode.ground_angle,UPRISE_L,MOTOR_MAX_ID);
      clamp_record_time= HAL_GetTick();
      action_one_state=ACTIONING;
      action_two_state=ACTIONING;
			ALL_ON
    }break;
    case ACTION_ONE:
    {
      if(action_one_state==ACTIONING)
      {
				if((HAL_GetTick()-clamp_record_time)==200)
				{
					Motor_uprise_angle_change(-1500);
				}
				if((HAL_GetTick()-clamp_record_time)==2000)
				{
					Motor_uprise_angle_change(1500);
				}
				else if((HAL_GetTick()-clamp_record_time)>=3500)
				{
					action_one_state=ACTION_DONE;
				}
      }
      else
        clamp_record_time= HAL_GetTick();
    }break;
    case ACTION_TWO:
    {
      if(action_two_state==ACTIONING)
        action_two_state=store_handler();
			else
				clamp_action=CLAMP_UN_CMD;
    }break;
  }
}

uint8_t ready_to_del_box_number_flag=1;
uint16_t test_flag=0;
void exchange_handler(void)
{
  switch(exchange_action)
  {
    case EXCHANGE_UN_CMD:
    {
			uint32_t clamp_record_time_test;
			EXCHANGE_PUMP_ON
			EXCHANGE_Solenoid_ON
      Motor_change_mode_angle(&Mode.exchange_angle,UPRISE_L,MOTOR_MAX_ID);
      clamp_record_time= HAL_GetTick();
	
			action_one_state=ACTIONING;
      action_two_state=ACTIONING;
			ready_to_del_box_number_flag=1;
    }break;
    case PICK_ACTION:
    {
			test_flag=0;
			 
        if(action_two_state==ACTIONING)
				{
					memset(&kb_adrust,0,sizeof(kb_adrust));
					action_two_state=pick_handler();
					if(test_time_flag)
					{
						CLAMP_Solenoid_OFF
						
					}
				}
				else
				{
					exchange_action=EXCHANGE_UN_CMD;
					clamp_record_time= HAL_GetTick();
				}
    }break;
    case RELESE_BOX_ACTION:
    {
			EXCHANGE_OFF
			if(ready_to_del_box_number_flag)
			{
				have_box_number--;
				clamp_record_time= HAL_GetTick();
			}
  
		}break;
  }
}

uint8_t store_handler(void)
{
	uint8_t finish_flag=0;
	uint32_t store_action_times_test;
	store_action_times_test=HAL_GetTick()-clamp_record_time;
	test_time=HAL_GetTick();
    
	  
//			if(chassis_mode==CHASSIS_CLAMP_BIG_ISLAND_SLANTED_MODE||chassis_mode==CHASSIS_CLAMP_BIG_ISLAND_STRAIGHT_MODE)
//			{
//				if(store_action_times_test==10)
//					Motor_change_mode_angle(&Mode.store_angle,UPRISE_L,MOTOR_MAX_ID);
//				if(store_action_times_test==2500)
//				{
//					Motor[CLAMP_PITCH].Angle.mode_angle-=5;
//					Motor_uprise_angle_change(-2400);
//				}
//				if(store_action_times_test==3500)
//				{
//					//CLAMP_OFF
//					CLAMP_Solenoid_OFF
//				}
//				if(store_action_times_test==5500)
//				{
//					Motor_uprise_angle_change(3000);
//				}
//				if(store_action_times_test>=6500)
//				{
//					finish_flag=1;
//				}
//			}			
			 if(chassis_mode==CHASSIS_CLAMP_SMALL_ISLAND_MODE||chassis_mode==CHASSIS_CLAMP_BIG_ISLAND_SLANTED_MODE||chassis_mode==CHASSIS_CLAMP_BIG_ISLAND_STRAIGHT_MODE)
		 {
			 if(store_action_times_test==10)
			 {
					Motor_change_mode_angle(&Mode.store_smallisland_angle,UPRISE_L,MOTOR_MAX_ID);
				
				 
			 }
				
				 if(store_action_times_test==4000)
				{

					Motor[CLAMP_PITCH].Angle.mode_angle-=80;
					
				}
				 if(store_action_times_test==5000)
				{
					
					CLAMP_Solenoid_OFF
					
				}
				
				 if(store_action_times_test==7000)
				{
					finish_flag=1;
					
				}
				
			}
		
		
//		case 2:
//		{
//			if(store_action_times>=1)
//				{
//					finish_flag=1;
//				}
//		}
//	}
	return finish_flag;
}

uint8_t pick_handler(void)
{		
	uint8_t finish_flag=0;
	uint32_t pick_action_times;
	pick_action_times=HAL_GetTick()-clamp_record_time;
	if(pick_action_times==10)
	{
		Motor_change_mode_angle(&Mode.exchange_pick_angle,UPRISE_L,MOTOR_MAX_ID);

//		Motor_uprise_angle_change(2600);
	}
	
	if(pick_action_times==4000)
	{
		
			Motor[CLAMP_PITCH].Angle.mode_angle-=80;
		//CLAMP_Solenoid_OFF
	}
	if(pick_action_times==5000)
	{
		CLAMP_Solenoid_OFF
	}
	

	if(pick_action_times>=7500)
	{
		//CLAMP_PUMP_ON
		if(have_box_number>0)have_box_number--;
		finish_flag=1;
	
	}
	return finish_flag;
}


void check_handler(void)
{
	Motor_change_mode_angle(&Mode.check_angle,UPRISE_L,MOTOR_MAX_ID);
}

//通过地址操作写指定范围电机mode_angle为当前模式角度
void Motor_change_mode_angle(int16_t* Mode_now,uint8_t low_ID,uint8_t hight_ID)
{
	uint8_t ID;
	uint32_t mode_angle_adress;
	uint32_t now_angle_adress;
	uint32_t target_adress;
	mode_angle_adress=(uint32_t)&Mode.mode_angle;
	now_angle_adress=(uint32_t)Mode_now;
	for(ID=low_ID;ID<=hight_ID;ID++)
	{
			target_adress=((uint32_t)&Motor[ID].Angle.mode_angle + now_angle_adress - mode_angle_adress);
			Motor[ID].Angle.mode_angle=*(int16_t*)target_adress;
	}
}

void Motor_uprise_angle_change(int16_t change_displacement)
{
	int16_t change_angle;
	change_angle=change_displacement*UPRISE_DISPLACEMENT_TO_ANGLE;
	Motor[UPRISE_L].Angle.mode_angle+=change_angle;
	Motor[UPRISE_R].Angle.mode_angle+=change_angle;
}

void Motor_slide_angle_change(int16_t change_displacement)
{
	int16_t change_angle;
	change_angle=change_displacement*SLIDE_DISPLACEMENT_TO_ANGLE;
	Motor[SLIDE_L].Angle.mode_angle+=change_angle;
	Motor[SLIDE_R].Angle.mode_angle-=change_angle;
}

static uint8_t Motor_current_limit_init(void)
{
	uint8_t ID;
	Motor_current_limit[SLIDE_CLAMP].special=5+00;
	Motor_current_limit[SLIDE_L].special=1000;
	Motor_current_limit[SLIDE_R].special=1000;
	for(ID=0;ID<MOTOR_NUMBER;ID++)
	{
		if(Whether_Brushless_Motor(Motor[ID]))
		{
			Motor_current_limit[ID].normal=Motor[ID].Brushless.speed_pid.maxout;
		}
	}
	
	return 1;
}

static void Motor_current_limit_reset(void)
{
	uint8_t ID;
	for(ID=0;ID<MOTOR_NUMBER;ID++)
	{
		if(Whether_Brushless_Motor(Motor[ID]))
		{
			Motor_current_limit[ID].flag=0;
	
		}
	}
}

static void Motor_current_limit_handler(void)
{
	uint8_t ID;
	for(ID=0;ID<MOTOR_NUMBER;ID++)
	{
		if(Whether_Brushless_Motor(Motor[ID]))
		{
			if(Motor_current_limit[ID].flag)
				Motor[ID].Brushless.speed_pid.maxout=Motor_current_limit[ID].special;
			else
				Motor[ID].Brushless.speed_pid.maxout=Motor_current_limit[ID].normal;
		}
	}
}













