/*****************************************************
	  �������д��2024/4/-
		
		��ȥ������һ�������Ҫȥbsp_can�ļ��������ո�ʽ��
ȥcommon�ĽǶ�ת�����뷢�͸�ʽ��ȥ��Ӧ�����ļ��������ĳ�ʼ��
		��ȥ����е����̧��������Ĵ����߼�����һ�£���������������
	  
		��ͳһmanipulator��slide��chassis��������߼���
��������ĵ��ID���ٶ���ǶȻ�ѡ���ȫ����ͷ�ļ�ͳһ�޸�
ʣ�½����������д���

		Ϊͳһ���õ�ʱ�������ɣ�
*****************************************************/
#include "motor_task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "comm_task.h"
#include "modeswitch_task.h"
#include "clamp.h"
int16_t test_angle1=2500;
int16_t small_test1=100;
UBaseType_t Motor_stack_surplus;
extern TaskHandle_t can_msg_send_Task_Handle;

motor_t Motor[MOTOR_NUMBER];

uint8_t Motor_init_state=INIT_NEVER;
uint8_t All_Offset_Angle_init_state=0;

uint32_t Servo_Test;
uint32_t Servo_pitch_test;
//�����ʼ�����
void Motor_base_init()
{
	motor_t Motor_Struct;
	//����ճ����ֻ�ø�һ�����־���������Ӧ�����������ǰǿ������
	{// CHASSIS INIT
    Motor_Struct.MOTOR_NAME = CHASSIS_FR;
    Motor_Struct.MOTOR_TYPE = M3508;
    Motor_Struct.Brushless.CAN_ID = 1;
    Motor_Struct.Brushless.ESC_ID = 1;
    Motor_Struct.Brushless.speed = 0; // ��ǶȲ����ٶ�
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = 0;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// UPRISE INIT
    Motor_Struct.MOTOR_NAME = UPRISE_L;
    Motor_Struct.MOTOR_TYPE = M3508;
    Motor_Struct.Brushless.CAN_ID = 1;
    Motor_Struct.Brushless.ESC_ID = 5;
		Motor_Struct.Brushless.Angle_to_Speed_mode=0;//���л�
    Motor_Struct.Brushless.speed = 1500; // ��ǶȲ����ٶ�
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = -700;
		

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// CLAMP_YAW INIT
    Motor_Struct.MOTOR_NAME = CLAMP_YAW;
    Motor_Struct.MOTOR_TYPE = SERVO;
    Motor_Struct.Servo.TIM = TIM2;
    Motor_Struct.Servo.Compare = 3; // PA2
    Motor_Struct.Servo.Rotation_range = 270;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// CLAMP_PITCH INIT
    Motor_Struct.MOTOR_NAME = CLAMP_PITCH;
    Motor_Struct.MOTOR_TYPE = SERVO;
    Motor_Struct.Servo.TIM = TIM4;
    Motor_Struct.Servo.Compare = 3; // PD14
    Motor_Struct.Servo.Rotation_range = 270;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// CLAMP_ROLL INIT
    Motor_Struct.MOTOR_NAME = CLAMP_ROLL;
    Motor_Struct.MOTOR_TYPE = SERVO;
    Motor_Struct.Servo.TIM = TIM4;
    Motor_Struct.Servo.Compare = 2; // PD13
    Motor_Struct.Servo.Rotation_range = 0;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// SLIDE INIT
    Motor_Struct.MOTOR_NAME = SLIDE_L;
    Motor_Struct.MOTOR_TYPE = M3508;
    Motor_Struct.Brushless.CAN_ID = 2;
    Motor_Struct.Brushless.ESC_ID = 1;
    Motor_Struct.Brushless.speed = 1000; // ��ǶȲ����ٶ�
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = 1000;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// SLIDE_CLAMP INIT
    Motor_Struct.MOTOR_NAME = SLIDE_CLAMP;
    Motor_Struct.MOTOR_TYPE = M2006;
    Motor_Struct.Brushless.CAN_ID = 2;
    Motor_Struct.Brushless.ESC_ID = 3;
    Motor_Struct.Brushless.speed = 0; // ��ǶȲ����ٶ�
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = 500;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// EXCHANGE_YAW INIT
    Motor_Struct.MOTOR_NAME = EXCHANGE_YAW;
    Motor_Struct.MOTOR_TYPE = M6020;
    Motor_Struct.Brushless.CAN_ID = 2;
    Motor_Struct.Brushless.ESC_ID = 5;
    Motor_Struct.Brushless.speed = 0; // ��ǶȲ����ٶ�
		
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = 120;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// EXCHANGE_PITCH INIT
    Motor_Struct.MOTOR_NAME = EXCHANGE_PITCH;
    Motor_Struct.MOTOR_TYPE = M2006;
    Motor_Struct.Brushless.CAN_ID = 2;
    Motor_Struct.Brushless.ESC_ID = 6;
    Motor_Struct.Brushless.speed = 0; // ��ǶȲ����ٶ�
    Motor_Struct.Brushless.state = 0;
    Motor_Struct.Brushless.offset_angle_init_flag = 0;
		Motor_Struct.Brushless.offset_angle_init_speed = 1000;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}

	{// EXCHANGE_ROLL INIT
    Motor_Struct.MOTOR_NAME = EXCHANGE_ROLL;
    Motor_Struct.MOTOR_TYPE = SERVO;
    Motor_Struct.Servo.TIM = TIM4;
    Motor_Struct.Servo.Compare = 4; // PD15
    Motor_Struct.Servo.Rotation_range = 270;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}
	
	{// GIMBAL_YAW
    Motor_Struct.MOTOR_NAME = GIMBAL_YAW;
    Motor_Struct.MOTOR_TYPE = SERVO;
    Motor_Struct.Servo.TIM = TIM2;
    Motor_Struct.Servo.Compare = 1; // PA0
    Motor_Struct.Servo.Rotation_range = 270;

    memcpy(&Motor[Motor_Struct.MOTOR_NAME], &Motor_Struct, sizeof(Motor_Struct));
    memset(&Motor_Struct, 0, sizeof(Motor_Struct));
	}
	
	Motor_pid_init(DONE);
	Motor_angle_init();
	
	Motor_base_init_copy(CHASSIS_FR,CHASSIS_BR);//���ø���
	Motor_base_init_copy(UPRISE_L,UPRISE_R);//���ø���
	Motor_base_init_copy(SLIDE_L,SLIDE_R);//���ø���
	
	Motor_base_init_reversal(SLIDE_R);//��תSLIDE_R����
//	Motor[SLIDE_R].MOTOR_TURN_FLAG=M_TURN;
}

//��ʼ��pid
void Motor_pid_init(INIT_STATUS init_status) 
{
	pid_motor_parameter_t PID_Motor_parameter_Struct;

	// CHASSIS_FR
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = CHASSIS_FR;
    PID_Motor_parameter_Struct.angle.p = 60.0;
    PID_Motor_parameter_Struct.angle.i = 0.01;
    PID_Motor_parameter_Struct.angle.d = 0.1;
    PID_Motor_parameter_Struct.angle.max_out = 30000;
    PID_Motor_parameter_Struct.angle.integral_limit = 30000;

    PID_Motor_parameter_Struct.speed.p = 60.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 30000;
    PID_Motor_parameter_Struct.speed.integral_limit = 30000;

    Motor_PID_Struct_Init(&Motor[CHASSIS_FR], PID_Motor_parameter_Struct, init_status);
	}

	// UPRISE_L
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = UPRISE_L;
    PID_Motor_parameter_Struct.angle.p = 30;
    PID_Motor_parameter_Struct.angle.i = 0.2;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 6000;
    PID_Motor_parameter_Struct.angle.integral_limit = 500;

    PID_Motor_parameter_Struct.speed.p = 20.0;
    PID_Motor_parameter_Struct.speed.i = 0.001;
    PID_Motor_parameter_Struct.speed.d = 0;
    PID_Motor_parameter_Struct.speed.max_out = 8000;
    PID_Motor_parameter_Struct.speed.integral_limit = 500;

    Motor_PID_Struct_Init(&Motor[UPRISE_L], PID_Motor_parameter_Struct, init_status);
	}

	// CLAMP_YAW
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = CLAMP_YAW;
    PID_Motor_parameter_Struct.angle.p = 0;
    PID_Motor_parameter_Struct.angle.i = 0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 0;
    PID_Motor_parameter_Struct.angle.integral_limit = 0;

    PID_Motor_parameter_Struct.speed.p = 60.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 1000;
    PID_Motor_parameter_Struct.speed.integral_limit = 1000;

    Motor_PID_Struct_Init(&Motor[CLAMP_YAW], PID_Motor_parameter_Struct, init_status);
	}

	// CLAMP_PITCH
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = CLAMP_PITCH;
    PID_Motor_parameter_Struct.angle.p = 0;
    PID_Motor_parameter_Struct.angle.i = 0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 0;
    PID_Motor_parameter_Struct.angle.integral_limit = 0;

    PID_Motor_parameter_Struct.speed.p = 60.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 30000;
    PID_Motor_parameter_Struct.speed.integral_limit = 30000;

    Motor_PID_Struct_Init(&Motor[CLAMP_PITCH], PID_Motor_parameter_Struct, init_status);
	}

	// CLAMP_ROLL
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = CLAMP_ROLL;
    PID_Motor_parameter_Struct.angle.p = 0;
    PID_Motor_parameter_Struct.angle.i = 0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 0;
    PID_Motor_parameter_Struct.angle.integral_limit = 0;

    PID_Motor_parameter_Struct.speed.p = 60.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 30000;
    PID_Motor_parameter_Struct.speed.integral_limit = 30000;

    Motor_PID_Struct_Init(&Motor[CLAMP_ROLL], PID_Motor_parameter_Struct, init_status);
	}

	// SLIDE_L
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = SLIDE_L;
    PID_Motor_parameter_Struct.angle.p = 20;
    PID_Motor_parameter_Struct.angle.i = 0.1;
    PID_Motor_parameter_Struct.angle.d = 0.02;
    PID_Motor_parameter_Struct.angle.max_out = 2000;
    PID_Motor_parameter_Struct.angle.integral_limit = 500;

    PID_Motor_parameter_Struct.speed.p = 20;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.02;
    PID_Motor_parameter_Struct.speed.max_out = 4500;
    PID_Motor_parameter_Struct.speed.integral_limit = 500;

    Motor_PID_Struct_Init(&Motor[SLIDE_L], PID_Motor_parameter_Struct, init_status);
	}
	// SLIDE_CLMP
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = SLIDE_CLAMP;
    PID_Motor_parameter_Struct.angle.p = 90;
    PID_Motor_parameter_Struct.angle.i = 0.0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 7000;
    PID_Motor_parameter_Struct.angle.integral_limit = 500;

    PID_Motor_parameter_Struct.speed.p = 20.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.;
    PID_Motor_parameter_Struct.speed.max_out = 5000;
    PID_Motor_parameter_Struct.speed.integral_limit = 1500;

    Motor_PID_Struct_Init(&Motor[SLIDE_CLAMP], PID_Motor_parameter_Struct, init_status);
	}
	
	// EXCHANGE_YAW
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = EXCHANGE_YAW;
    PID_Motor_parameter_Struct.angle.p = 15;
    PID_Motor_parameter_Struct.angle.i = 0.0;
    PID_Motor_parameter_Struct.angle.d = 0.0;
    PID_Motor_parameter_Struct.angle.max_out = 1000;
    PID_Motor_parameter_Struct.angle.integral_limit = 30000;

    PID_Motor_parameter_Struct.speed.p = 80.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 30000;
    PID_Motor_parameter_Struct.speed.integral_limit = 30000;

    Motor_PID_Struct_Init(&Motor[EXCHANGE_YAW], PID_Motor_parameter_Struct, init_status);
	}

	// EXCHANGE_PITCH
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = EXCHANGE_PITCH;
    PID_Motor_parameter_Struct.angle.p = 75;
    PID_Motor_parameter_Struct.angle.i = 0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 2500;
    PID_Motor_parameter_Struct.angle.integral_limit = 1000;

    PID_Motor_parameter_Struct.speed.p = 20.0;
    PID_Motor_parameter_Struct.speed.i = 0;
    PID_Motor_parameter_Struct.speed.d = 0;
    PID_Motor_parameter_Struct.speed.max_out = 7000;
    PID_Motor_parameter_Struct.speed.integral_limit = 6000;

    Motor_PID_Struct_Init(&Motor[EXCHANGE_PITCH], PID_Motor_parameter_Struct, init_status);
	}

	// EXCHANGE_ROLL
	{
    PID_Motor_parameter_Struct.MOTOR_NAME = EXCHANGE_ROLL;
    PID_Motor_parameter_Struct.angle.p = 0;
    PID_Motor_parameter_Struct.angle.i = 0;
    PID_Motor_parameter_Struct.angle.d = 0;
    PID_Motor_parameter_Struct.angle.max_out = 0;
    PID_Motor_parameter_Struct.angle.integral_limit = 0;

    PID_Motor_parameter_Struct.speed.p = 60.0;
    PID_Motor_parameter_Struct.speed.i = 0.01;
    PID_Motor_parameter_Struct.speed.d = 0.1;
    PID_Motor_parameter_Struct.speed.max_out = 30000;
    PID_Motor_parameter_Struct.speed.integral_limit = 30000;

    Motor_PID_Struct_Init(&Motor[EXCHANGE_ROLL], PID_Motor_parameter_Struct, init_status);
	}
}

//ģʽ�Ƕȳ�ʼ��
void Motor_angle_init()
{
	// normal_angle
	{
		Motor[CHASSIS_FR].Angle.normal_angle = 0;
		Motor[UPRISE_L].Angle.normal_angle = 1000;//1000;MAX7000
		Motor[CLAMP_YAW].Angle.normal_angle = 30;//
		Motor[CLAMP_PITCH].Angle.normal_angle = 120;//120
		Motor[CLAMP_ROLL].Angle.normal_angle = 0;
		Motor[SLIDE_L].Angle.normal_angle = 0;//-100MAX:-850
		Motor[SLIDE_CLAMP].Angle.smallisland_angle = 0;
		Motor[EXCHANGE_YAW].Angle.normal_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.normal_angle = -2000;//2200ˮƽ��MAX3500
		Motor[EXCHANGE_ROLL].Angle.normal_angle = 90;
		Motor[GIMBAL_YAW].Angle.normal_angle = NORMAL_LOOK;//62
	}

	// bigisland_straight_angle
	{
		Motor[CHASSIS_FR].Angle.bigisland_straight_angle = 0;
		Motor[UPRISE_L].Angle.bigisland_straight_angle = 2000;
		Motor[CLAMP_YAW].Angle.bigisland_straight_angle = 0;
		Motor[CLAMP_PITCH].Angle.bigisland_straight_angle = 10;
		Motor[CLAMP_ROLL].Angle.bigisland_straight_angle = 0;
		Motor[SLIDE_L].Angle.bigisland_straight_angle = 0;
		Motor[SLIDE_CLAMP].Angle.bigisland_straight_angle = -1350;//MAX-1950
		Motor[EXCHANGE_YAW].Angle.bigisland_straight_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.bigisland_straight_angle = 0;
		Motor[EXCHANGE_ROLL].Angle.bigisland_straight_angle = 0;
		Motor[GIMBAL_YAW].Angle.bigisland_straight_angle = MODE_LOOK;//62
	}

	// bigisland_slanted_angle
	{
		Motor[CHASSIS_FR].Angle.bigisland_slanted_angle = 0;
		Motor[UPRISE_L].Angle.bigisland_slanted_angle = 2000;
		Motor[CLAMP_YAW].Angle.bigisland_slanted_angle = 80;
		Motor[CLAMP_PITCH].Angle.bigisland_slanted_angle = 10;
		Motor[CLAMP_ROLL].Angle.bigisland_slanted_angle = 0;
		Motor[SLIDE_L].Angle.bigisland_slanted_angle = 0;
		Motor[SLIDE_CLAMP].Angle.bigisland_slanted_angle = -1350;
		Motor[EXCHANGE_YAW].Angle.bigisland_slanted_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.bigisland_slanted_angle = 0;
		Motor[EXCHANGE_ROLL].Angle.bigisland_slanted_angle = 0;
		Motor[GIMBAL_YAW].Angle.bigisland_slanted_angle = MODE_LOOK;//62
	}

	//smallisland_angle
	{
		Motor[CHASSIS_FR].Angle.smallisland_angle = 0;
		Motor[UPRISE_L].Angle.smallisland_angle = 2500;//1300; //1000;
		Motor[CLAMP_YAW].Angle.smallisland_angle = 0;
		Motor[CLAMP_PITCH].Angle.smallisland_angle = 0;
		Motor[CLAMP_ROLL].Angle.smallisland_angle = 0;
		Motor[SLIDE_L].Angle.smallisland_angle = -280;//-120;//300
		Motor[SLIDE_CLAMP].Angle.smallisland_angle = 0;
		Motor[EXCHANGE_YAW].Angle.smallisland_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.smallisland_angle = -2500;
		Motor[EXCHANGE_ROLL].Angle.smallisland_angle = 0;
		Motor[GIMBAL_YAW].Angle.smallisland_angle = MODE_LOOK;//62
	}
	//smallisland_angle_2
	{
		Motor[CHASSIS_FR].Angle.smallisland_angle_2 = 0;
		Motor[UPRISE_L].Angle .smallisland_angle_2 = 1000;//1300; //1000;
		Motor[CLAMP_YAW].Angle.smallisland_angle_2 = 0;
		Motor[CLAMP_PITCH].Angle.smallisland_angle_2 = 0;
		Motor[CLAMP_ROLL].Angle.smallisland_angle_2 = 0;
		Motor[SLIDE_L].Angle.smallisland_angle_2 = -100;//300
		Motor[SLIDE_CLAMP].Angle.smallisland_angle_2 = 0;
		Motor[EXCHANGE_YAW].Angle.smallisland_angle_2 = 0;
		Motor[EXCHANGE_PITCH].Angle.smallisland_angle_2 =0;//200
		Motor[EXCHANGE_ROLL].Angle.smallisland_angle_2 = 0;
		Motor[GIMBAL_YAW].Angle.smallisland_angle_2 = MODE_LOOK;//62
	}
	// ground_angle
	{
		Motor[CHASSIS_FR].Angle.ground_angle = 0;
		Motor[UPRISE_L].Angle.ground_angle = 500;
		Motor[CLAMP_YAW].Angle.ground_angle = 0;
		Motor[CLAMP_PITCH].Angle.ground_angle = 100;
		Motor[CLAMP_ROLL].Angle.ground_angle = 0;
		Motor[SLIDE_L].Angle.ground_angle = 0;
		Motor[SLIDE_CLAMP].Angle.ground_angle = -900;//-650
		Motor[EXCHANGE_YAW].Angle.ground_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.ground_angle = 0;//0
		Motor[EXCHANGE_ROLL].Angle.ground_angle = 0;
		Motor[GIMBAL_YAW].Angle.ground_angle = MODE_LOOK;//62
	}

	// exchange_angle
	{
		Motor[CHASSIS_FR].Angle.exchange_angle = 0;
		Motor[UPRISE_L].Angle.exchange_angle = 6000;
		Motor[CLAMP_YAW].Angle.exchange_angle = -70;
		Motor[CLAMP_PITCH].Angle.exchange_angle = 0;
		Motor[CLAMP_ROLL].Angle.exchange_angle = 0;
		Motor[SLIDE_L].Angle.exchange_angle = -500;
		Motor[SLIDE_CLAMP].Angle.exchange_angle = 0;
		Motor[EXCHANGE_YAW].Angle.exchange_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.exchange_angle = 0;
		Motor[EXCHANGE_ROLL].Angle.exchange_angle = 0;
		Motor[GIMBAL_YAW].Angle.exchange_angle = MODE_LOOK;//62
	}

	// store_angle
	{
		Motor[CHASSIS_FR].Angle.store_angle = 0;
		Motor[UPRISE_L].Angle.store_angle = 0;
		Motor[CLAMP_YAW].Angle.store_angle = 0;
		Motor[CLAMP_PITCH].Angle.store_angle = 0;
		Motor[CLAMP_ROLL].Angle.store_angle = 0;
		Motor[SLIDE_L].Angle.store_angle = 0;
		Motor[SLIDE_CLAMP].Angle.store_angle = 0;
		Motor[EXCHANGE_YAW].Angle.store_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.store_angle = -100;
		Motor[EXCHANGE_ROLL].Angle.store_angle = 0;//180;
		Motor[GIMBAL_YAW].Angle.store_angle = MODE_LOOK;//62
	}

	// exchange_pick_angle
	{
		Motor[CHASSIS_FR].Angle.exchange_pick_angle = 0;
		Motor[UPRISE_L].Angle.exchange_pick_angle = -1000;
		Motor[CLAMP_YAW].Angle.exchange_pick_angle = 0;
		Motor[CLAMP_PITCH].Angle.exchange_pick_angle = 0;
		Motor[CLAMP_ROLL].Angle.exchange_pick_angle = 0;
		Motor[SLIDE_L].Angle.exchange_pick_angle = -130;
		Motor[SLIDE_CLAMP].Angle.exchange_pick_angle = 0;
		Motor[EXCHANGE_YAW].Angle.exchange_pick_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.exchange_pick_angle = 100;
		Motor[EXCHANGE_ROLL].Angle.exchange_pick_angle = 180;
		Motor[GIMBAL_YAW].Angle.exchange_pick_angle = MODE_LOOK;//62
	}
	{
		Motor[CHASSIS_FR].Angle.store_smallisland_angle = 0;
		Motor[UPRISE_L].Angle.store_smallisland_angle = -1000;
		Motor[CLAMP_YAW].Angle.store_smallisland_angle = 0;
		Motor[CLAMP_PITCH].Angle.store_smallisland_angle = 0;
		Motor[CLAMP_ROLL].Angle.store_smallisland_angle = 0;
		Motor[SLIDE_L].Angle.store_smallisland_angle = -130;
		Motor[SLIDE_CLAMP].Angle.store_smallisland_angle =0;
		Motor[EXCHANGE_YAW].Angle.store_smallisland_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.store_smallisland_angle = 100;//-100
		Motor[EXCHANGE_ROLL].Angle.store_smallisland_angle = 0;
		Motor[GIMBAL_YAW].Angle.store_smallisland_angle = MODE_LOOK;//62
	}
	{
		Motor[CHASSIS_FR].Angle.check_angle = 0;
		Motor[UPRISE_L].Angle.check_angle = 6000;
		Motor[CLAMP_YAW].Angle.check_angle = 0;
		Motor[CLAMP_PITCH].Angle.check_angle = 0;
		Motor[CLAMP_ROLL].Angle.check_angle = 0;
		Motor[SLIDE_L].Angle.check_angle = 0;
		Motor[SLIDE_CLAMP].Angle.check_angle = 0;
		Motor[EXCHANGE_YAW].Angle.check_angle = 0;
		Motor[EXCHANGE_PITCH].Angle.check_angle = 0;
		Motor[EXCHANGE_ROLL].Angle.check_angle = 0;
		Motor[GIMBAL_YAW].Angle.check_angle = MODE_LOOK;//62
	}
}

void Motor_task(void *parm)
{
	uint32_t Signal;
	BaseType_t STAUS;
	uint8_t ID;
	
	while(1)
	{
		STAUS = xTaskNotifyWait((uint32_t) NULL, 
								(uint32_t) INFO_GET_MOTOR_SIGNAL, 
								(uint32_t *)&Signal, 
								(TickType_t) portMAX_DELAY );
		if(STAUS ==pdTRUE)
		{
			if(Motor_init_state==INIT_NEVER)//δ���ò�������в�������
			{
				Motor_base_init();
				Motor_init_state=INIT_DONE;
			}
			for(ID=0;ID<MOTOR_NUMBER;ID++)//�������
			{
				if(Whether_Brushless_Motor(Motor[ID])==0)
				{
					Motor_Servo_handler(ID);
				}
			}
			
			if((Signal & INFO_GET_MOTOR_SIGNAL) && chassis_mode != CHASSIS_RELEASE)
			{
				/******************��ʽ��ʼ*****************************/
				if(All_Offset_Angle_init_state==0)
				{
					Servo_angle_init();
					All_Offset_Angle_init_state=Motor_offset_angle_init();
					
				}//�ж�
				
				else//��ת����
				{
					clamp_angle_handle();
					for(ID=0;ID<MOTOR_NUMBER;ID++)//��ʽ����
					{
					if((Motor[ID].MOTOR_NAME>CHASSIS_BR) && (Whether_Brushless_Motor(Motor[ID])))//����û�ģ�����������м���
						{
			
							/******************�ٶȽǶȻ��л�����*****************************/
							Motor[ID].Brushless.Speed_or_Angle_flag=ANGLE_MODE;
							if(Motor[ID].Brushless.Angle_to_Speed_mode && All_Offset_Angle_init_state)
							{
								Motor[ID].Brushless.Speed_or_Angle_flag=SPEED_MODE;
								if(Motor[ID].Brushless.angle_ref-Motor[ID].Brushless.angle_fdb > 100)
								{
									Motor[ID].Brushless.spd_ref=  Motor[ID].Brushless.speed;
								}
								else if(Motor[ID].Brushless.angle_ref-Motor[ID].Brushless.angle_fdb < 100)
								{
									Motor[ID].Brushless.spd_ref= -Motor[ID].Brushless.speed;
								}
								else Motor[ID].Brushless.Speed_or_Angle_flag=ANGLE_MODE; 
							}

							Motor_pid_clac(ID);
							Motor_current_into_CAN(ID);
							
						}//����ȥ�����ŷ���ˢ���ȥ������
					}//ѭ������
				}//�ж�����
				
				
				/******************�������*****************************/
			}
				xTaskGenericNotify((TaskHandle_t) can_msg_send_Task_Handle, 
													 (uint32_t) INFO_SEND_MOTOR_SIGNAL, 
													 (eNotifyAction) eSetBits, 
													 (uint32_t *)NULL );
				
				Motor_stack_surplus = uxTaskGetStackHighWaterMark(NULL);    					
		}
	}
}


void Motor_base_init_copy(uint8_t low,uint8_t hight)
{
	uint8_t ID;
	uint8_t count=0;
	for(ID=low+1;ID<=hight;ID++)
	{
		memcpy(&Motor[ID],&Motor[low],sizeof(Motor[low]));
		if(Whether_Brushless_Motor(Motor[ID]))
		{
			count++;
			Motor[ID].MOTOR_NAME=(motor_name_status)ID;//����
			Motor[ID].Brushless.ESC_ID=Motor[low].Brushless.ESC_ID+count;//�ĵ��ID
		}
	}//����
	
}

//���ٶ�ȡ�������ұ�������ģʽ�Ƕ�
void Motor_base_init_reversal(uint8_t ID)
{
	int16_t* first_mode_angle_adress;
	uint8_t i;
	first_mode_angle_adress = &Motor[ID].Angle.normal_angle;
	
	Motor[ID].Brushless.offset_angle_init_speed=-Motor[ID].Brushless.offset_angle_init_speed;
	for(i=0;i<MODE_ANGLE_NUMBER;i++)
	{
		*first_mode_angle_adress = -*first_mode_angle_adress;
		first_mode_angle_adress++;
	}
	
}

void Motor_PID_Struct_Init(motor_t *Motor_recieve,pid_motor_parameter_t parameter_Struct,INIT_STATUS init_status)
{
	PID_Struct_Init(&Motor_recieve->Brushless.angle_pid,parameter_Struct.angle.p,parameter_Struct.angle.i,parameter_Struct.angle.d,
		parameter_Struct.angle.max_out,parameter_Struct.angle.integral_limit,init_status);
	PID_Struct_Init(&Motor_recieve->Brushless.speed_pid,parameter_Struct.speed.p,parameter_Struct.speed.i,parameter_Struct.speed.d,
		parameter_Struct.speed.max_out,parameter_Struct.speed.integral_limit,init_status);
}

uint8_t Motor_offset_angle_init(void)
{
	uint8_t ID;
	uint8_t all_init_state;
	all_init_state=1;
	for(ID=0;ID<MOTOR_NUMBER;ID++)//��ת��ʼ��
	{
		if(Motor[ID].MOTOR_NAME>CHASSIS_BR && Whether_Brushless_Motor(Motor[ID]))
		{
			if(Motor[ID].Brushless.offset_angle_init_flag==0)//����û�ģ�����������м���
			{
				if(Motor[ID].MOTOR_TYPE!=M6020)
					Motor[ID].Brushless.spd_ref=Motor[ID].Brushless.offset_angle_init_speed;
				else
				{
					if((Motor[ID].Brushless.ecd_fdb - 5790) > 0)
						Motor[ID].Brushless.spd_ref=-Motor[ID].Brushless.offset_angle_init_speed;
					else if((Motor[ID].Brushless.ecd_fdb - 5790) < 0)
						Motor[ID].Brushless.spd_ref=Motor[ID].Brushless.offset_angle_init_speed;
					else Motor[ID].Angle.offset_angle=Motor[ID].Brushless.angle_fdb;
				}
				
				if((fabs(Motor[ID].Brushless.spd_ref) - fabs(Motor[ID].Brushless.spd_fdb)) > 0.3*fabs(Motor[ID].Brushless.spd_ref))//�ж��Ƿ��ת��ռ��Խ������Խ���̣�
				{
					Motor[ID].Brushless.err_count++;
					if(Motor[ID].Brushless.err_count>200)
					{//�ﵽ��ת
						Motor[ID].Brushless.offset_angle_init_flag = 1;
						Motor[ID].Angle.offset_angle=Motor[ID].Brushless.angle_fdb;
						Motor[ID].Brushless.spd_ref=0;
						Motor[ID].Brushless.err_count=0;
					}
				}
				
//				else
//				{
//					Motor[ID].Brushless.spd_ref=0;
//					Motor[ID].Brushless.err_count=0;
//				}
			}//δ��ת�����
			Motor_pid_clac(ID);
			Motor_current_into_CAN(ID);
			if(Motor[ID].Brushless.offset_angle_init_flag==0) all_init_state=0;//��������ʼ��δ���
		}//ѡ�п��Ƶ��
	}//ѭ��

	return all_init_state;
}

uint8_t Servo_angle_init(void)
{
	
		Motor[CLAMP_PITCH].Brushless.angle_ref=100;
	
	
	
}
void Motor_Servo_handler(uint8_t ID)
{
	uint32_t TIM_out;
	TIM_out=((2000/Motor[ID].Servo.Rotation_range)*Motor[ID].Servo.angle_ref)+500;
	switch(Motor[ID].Servo.Compare)
	{
		case 1:
		{
			TIM_SetCompare1(Motor[ID].Servo.TIM,TIM_out);
		}break;
		case 2:
		{
			TIM_SetCompare2(Motor[ID].Servo.TIM,TIM_out);
		}break;
		case 3:
		{
			TIM_SetCompare3(Motor[ID].Servo.TIM,TIM_out);
		}break;
		case 4:
		{
			TIM_SetCompare4(Motor[ID].Servo.TIM,TIM_out);
		}break;
	}
}

void Motor_pid_clac(uint8_t ID)
{
	if(Motor[ID].Brushless.Speed_or_Angle_flag==ANGLE_MODE)
	{
		Motor[ID].Brushless.spd_ref=pid_calc(&Motor[ID].Brushless.angle_pid,Motor[ID].Brushless.angle_fdb,Motor[ID].Brushless.angle_ref);
		Motor[ID].Brushless.current_send=pid_calc(&Motor[ID].Brushless.speed_pid,Motor[ID].Brushless.spd_fdb,Motor[ID].Brushless.spd_ref);
	}
	else if(Motor[ID].Brushless.Speed_or_Angle_flag==SPEED_MODE)
	{
		Motor[ID].Brushless.current_send=pid_calc(&Motor[ID].Brushless.speed_pid,Motor[ID].Brushless.spd_fdb,Motor[ID].Brushless.spd_ref);
	}
//	if(Motor[ID].MOTOR_TURN_FLAG)
//	{
//		Motor[ID].Brushless.current_send=-Motor[ID].Brushless.current_send;
//	}
}
void Motor_current_into_CAN(uint8_t ID)
{
	switch(Motor[ID].Brushless.CAN_ID)
	{
		case 1:
		{
			memcpy(&CAN1_current[Motor[ID].Brushless.ESC_ID],&Motor[ID].Brushless.current_send,sizeof(Motor[ID].Brushless.current_send));
		}break;
		case 2:
		{
			memcpy(&CAN2_current[Motor[ID].Brushless.ESC_ID],&Motor[ID].Brushless.current_send,sizeof(Motor[ID].Brushless.current_send));
		}break;
	}
	
}

uint8_t Whether_Brushless_Motor(motor_t Motor)
{
	switch(Motor.MOTOR_TYPE)
	{
		case M6020:
		case M3508:
		case M2006:
			return 1;
	default:
	return 0;
	}
}











