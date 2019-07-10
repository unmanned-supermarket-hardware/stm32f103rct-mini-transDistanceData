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

extern u8 startOpen,startClose,isOpen;
extern u8 USART2_RX_FLAG;
extern u8 USART2_RX_BUF[64]; //接收到的数据
extern u8 USART3_RX_BUF[64]; //接收到的数据
extern u8 UART5_RX_BUF[64]; //接收到的数据
extern double d2;
extern double d3;
extern double d5;

void open()
{
			printf("turning on usart2...");
			delay_ms(500);
			usart2_sendString("iFACM:0",7);
			delay_ms(500);
			usart2_sendString("iFACM:0",7);
			delay_ms(500);
			
			printf("turning on usart3...");
			delay_ms(500);
			usart3_sendString("iFACM:0",7);
			delay_ms(500);
			usart3_sendString("iFACM:0",7);
			delay_ms(500);
			
			printf("turning on uart5...");
			delay_ms(500);
			uart5_sendString("iFACM:0",7);
			delay_ms(500);
			uart5_sendString("iFACM:0",7);
			delay_ms(500);
			
			printf("finished...USART2_RX_FLAG = %d ",USART2_RX_FLAG);
			delay_ms(500);
			startOpen = 0;
			isOpen = 1;
}


int main(void)
{			

	u16 jsonSize;
	cJSON *root;
	char *strJson;
	char strSend[256];
	
	strSend[0] = '#';
	strSend[1] = '!';
	strSend[2] = 0;
	
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart5_init(36,115200);

	delay_ms(1000);
	
	
	open();

	while(1)
	{


		if(startOpen)
		{
			open();
		}
		
		if(startClose)
		{
			usart2_sendString("iHALT",5);
			delay_ms(100);
			usart3_sendString("iHALT",5);
			delay_ms(100);
			uart5_sendString("iHALT",5);
			delay_ms(100);
			startClose = 0;
			isOpen = 0;
		}
					
		//--------------------
		if(isOpen)
		{
			root=cJSON_CreateObject();

			cJSON_AddNumberToObject(root,"from",1);
			cJSON_AddNumberToObject(root,"to",2);
			cJSON_AddNumberToObject(root,"msType",1);
			
			cJSON_AddNumberToObject(root,"F", d2);
			cJSON_AddNumberToObject(root,"L1", d3);
			cJSON_AddNumberToObject(root,"L2", d5);
			
			cJSON_AddStringToObject(root,"d2str",(char *)USART2_RX_BUF);
			cJSON_AddStringToObject(root,"d3str",(char *)USART3_RX_BUF);
			cJSON_AddStringToObject(root,"d5str",(char *)UART5_RX_BUF);
			
		
			strJson=cJSON_Print(root); 
			cJSON_Delete(root); 
			
			jsonSize = strlen(strJson);

			strSend[3] = jsonSize;

			strncpy(strSend+4,strJson,strlen(strJson));
			//printf("%s\r\nlen = %d ,%x",strSend,jsonSize,jsonSize); 
			usart1_sendString(strSend,4 + jsonSize);

			
			myfree(strJson);
			delay_ms(500);
		}
		
	}	 
}














