#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "key.h" 
//ALIENTEK Mini STM32开发板范例代码3
//串口实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司
# define DATA_LEN 13
extern u8 USART2_RX_BUF[64]; //接收到的数据
extern u8 USART3_RX_BUF[64]; //接收到的数据
extern u8 UART5_RX_BUF[64]; //接收到的数据
int main(void)
{			
	char strTemp[64];
	char *p2 = strTemp;
	char *p3 = strTemp +DATA_LEN;
	char *p5 = strTemp +DATA_LEN*2;
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,38400);	 
	uart2_init(36,38400);
	uart3_init(36,38400);
	uart5_init(36,38400);


	while(1)
	{

		delay_ms(100);
		strncpy(p2,(char *)USART2_RX_BUF,DATA_LEN);
		strncpy(p3,(char *)USART3_RX_BUF,DATA_LEN);
		strncpy(p5,(char *)UART5_RX_BUF,DATA_LEN);
		strcpy(p5+DATA_LEN,"\r\n\r\n");

		usart1_sendString(strTemp,DATA_LEN*3+4);
		
	}	 
}














