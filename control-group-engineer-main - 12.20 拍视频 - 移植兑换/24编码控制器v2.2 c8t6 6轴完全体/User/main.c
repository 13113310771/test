/**
  ******************************************************************************
  * 函数库  ： 基于STM32F4标准库 
  * 芯片型号： C板
  ******************************************************************************
  *                          RM . 电控之歌
  *
  *                  一年备赛两茫茫，写程序，到天亮。
  *                      万行代码，Bug何处藏。
  *                  机械每天新想法，天天改，日日忙。
  *
  *                  视觉调试又怎样，朝令改，夕断肠。
  *                      相顾无言，惟有泪千行。
  *                  每晚灯火阑珊处，夜难寐，继续肝。
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"                  // Device header

/*config*/
#include "delay.h"


/*bsp*/
#include "bsp_usart.h"

/*usart*/

#include "transmit_task.h"
#include "conformity_task.h"
/*i2c*/
#include "as5600_task.h"
#include "i2c.h"


/*陀螺仪*/




int main(void)
{ 
	/*配置*/
	
	SysTick_Init(72);//1ms
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//优先级分组4：全为抢占优先级
	//TIM_BASE_Init(10-1,8400-1);//配置定时器4为1ms中断一次，做来系统詗诵屑剖阌笫迪忠恍邮辈僮
//	TIM_BASE_Init(10-1,9000-1);//90 000 000/9 000 = 10 000/10 = 1000HZ
	

	
//  TIM8_DEVICE(20000-1,168-1);//舵机PWM周期需要配置成20ms，舵机0-180°对应为高电平持续时间0.5ms-2.5ms
//	TIM1_DEVICE(2500-1,168-1);//摩擦轮
////	TIM12_DEVICE(400-1,90-1);//蜂鸣器，PWM频率在2700HZ时声音最大，且占空比需要设置成5％的高电平，此处设置为2500HZ
	
//  TIM10_DEVICE(5000-1,0);//提供一路PWM使得加热电阻升温，用于恒温加热IMU
//	
//	CAN1_DEVICE(CAN_Mode_Normal, CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 5);
//	CAN2_DEVICE(CAN_Mode_Normal, CAN_SJW_4tq, 
//	CAN_BS1_3tq, CAN_BS2_5tq, 5);
	
//	USART3_DEVICE();//用于遥控通信，采用的是DMA加空闲中断的方式接收数据
	USART1_DEVICE();//用于PC通信数据，采用的是DMA加空闲中断的方式接收数据
//  USART2_DEVICE();//用于裁判系统通信，采用的是DMA加空闲中断的方式接收数据;
	
//	SPI_DEVICE();//用于读取板载陀螺仪
	
//	Encoder_TIM1_Init();
//	Encoder_TIM8_Init();
	conformity_init();
	IIC_Init();
	
//	BMI088_init();

	
  while (1)
  {		
		
//		as5600_Init_task();
//		IIC_Test();
		
		AMS5600Registers_Read_task();
//		as5600_Test_task();
		as5600_task();
		
		model_task();
		transmit_task();
		delay_ms(40);//1000/40=25HZ
//		IIC_Test();
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
		
  }
}
#endif
