#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 
//ALIENTEK Mini STM32�����巶������3
//����ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
 
int main(void)
{			

	Stm32_Clock_Init(9); //ϵͳʱ������
	delay_init(72);	     //��ʱ��ʼ�� 
	uart_init(72,115200);	 //���ڳ�ʼ��Ϊ9600  ��Ϊ115200����
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart5_init(36,115200);

	while(1)
	{
//		usart1_sendString("hello",5);
//		usart2_sendString("22222",5);
//		delay_ms(500);
	}	 
}














