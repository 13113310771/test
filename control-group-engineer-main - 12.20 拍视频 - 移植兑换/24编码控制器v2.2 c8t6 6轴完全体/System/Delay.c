#include "stm32f10x.h"
#include "delay.h"


static uint8_t  fac_us=0;							//usÑÓÊ±±¶³ËÊý			   
static uint16_t fac_ms=0;							//msÑÓÊ±±¶³ËÊý
/**
  * @brief  ?????
  * @param  xus ????,??:0~233015
  * @retval ?
  */
	void SysTick_Init(uint8_t SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//SYSCLKµÄ8·ÖÆµ ±£´æ1usËùÐèµÄ¼ÆÊý´ÎÊý
	fac_ms=(uint16_t)fac_us*1000;				//Ã¿¸ömsÐèÒªµÄsystickÊ±ÖÓÊý   
}								    
void delay_us(uint32_t xus)
{
	SysTick->LOAD = fac_us * xus;				//????????
	SysTick->VAL = 0x00;					//???????
	SysTick->CTRL = 0x00000005;				//??????HCLK,?????
	while(!(SysTick->CTRL & 0x00010000));	//?????0
	SysTick->CTRL = 0x00000004;				//?????
}

/**
  * @brief  ?????
  * @param  xms ????,??:0~4294967295
  * @retval ?
  */
void delay_ms(uint32_t xms)
{
	while(xms--)
	{
		delay_us(1000);
	}
}
 
/**
  * @brief  ????
  * @param  xs ????,??:0~4294967295
  * @retval ?
  */
void delay_s(uint32_t xs)
{
	while(xs--)
	{
		delay_ms(1000);
	}
} 
