#include "receive_task.h"
int a=0;
//void USART1_IRQHandler(void)
//{
//	a=1;
//	
//}

/*USART1 中断函数*/
void USART1_IRQHandler(void)
{
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE) != RESET 
		 && USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART1);		
		USART_ClearFlag(USART1, USART_FLAG_IDLE);//清除空闲中断标志位
    
//    err_detector_hook(PC_SYS_OFFLINE);
    
//    if(pc_rx_Task_Handle != NULL)//避免任务没来得及创建就发送信号量，导致卡在断言机制中
//    {
//      xTaskNotifyFromISR((TaskHandle_t) pc_rx_Task_Handle, 
//                         (uint32_t) PC_UART_IDLE_SIGNAL, 
//                         (eNotifyAction) eSetBits, 
//                         (BaseType_t *)&xHigherPriorityTaskWoken);
//      
//      /*进行上下文切换*/
//      if(xHigherPriorityTaskWoken != pdFALSE)
//        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//    }
	}
}
