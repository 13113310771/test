#include "receive_task.h"
int a=0;
//void USART1_IRQHandler(void)
//{
//	a=1;
//	
//}

/*USART1 �жϺ���*/
void USART1_IRQHandler(void)
{
//  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
	if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE) != RESET 
		 && USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
	{
		USART_ReceiveData(USART1);		
		USART_ClearFlag(USART1, USART_FLAG_IDLE);//��������жϱ�־λ
    
//    err_detector_hook(PC_SYS_OFFLINE);
    
//    if(pc_rx_Task_Handle != NULL)//��������û���ü������ͷ����ź��������¿��ڶ��Ի�����
//    {
//      xTaskNotifyFromISR((TaskHandle_t) pc_rx_Task_Handle, 
//                         (uint32_t) PC_UART_IDLE_SIGNAL, 
//                         (eNotifyAction) eSetBits, 
//                         (BaseType_t *)&xHigherPriorityTaskWoken);
//      
//      /*�����������л�*/
//      if(xHigherPriorityTaskWoken != pdFALSE)
//        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//    }
	}
}
