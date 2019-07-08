#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 
//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
 
int main(void)
{			

	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 //串口初始化为9600  改为115200试试
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














