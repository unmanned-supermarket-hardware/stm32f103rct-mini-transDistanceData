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
	uart_init(72,9600);	 //串口初始化为9600  改为115200试试
	uart2_init(36,115200);
	uart3_init(36,115200);
	LED_Init();		  	 //初始化与LED连接的硬件接口    
	while(1)
	{
		
	}	 
}














