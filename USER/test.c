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
int main(void)
{			
	char strTemp[64];
	char *p2 = strTemp;
	char *p3 = strTemp +DATA_LEN;
	char *p5 = strTemp +DATA_LEN*2;
	
	cJSON *root;
	char *strJson;
	u8 head[3]; //head 格式为 ## 数据个数
	char strSend[256];
	
	head[0] = head[1] ='#';
	Stm32_Clock_Init(9); //系统时钟设置
	delay_init(72);	     //延时初始化 
	uart_init(72,115200);	 
	uart2_init(36,115200);
	uart3_init(36,115200);
	uart5_init(36,115200);

	delay_ms(100);
	usart2_sendString("iFACM:0",7);
	delay_ms(100);
	usart3_sendString("iFACM:0",7);
	delay_ms(100);
	uart5_sendString("iFACM:0",7);

	while(1)
	{

//		delay_ms(100);
//		strncpy(p2,(char *)USART2_RX_BUF,DATA_LEN);
//		strncpy(p3,(char *)USART3_RX_BUF,DATA_LEN);
//		strncpy(p5,(char *)UART5_RX_BUF,DATA_LEN);
//		strcpy(p5+DATA_LEN,"\r\n\r\n");

		//usart1_sendString(strTemp,DATA_LEN*3+4);
		
		//------------------test json
		if(startOpen)
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
			
			printf("turning on usart5...");
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


			cJSON_AddNumberToObject(root,"d2", d2);
			cJSON_AddNumberToObject(root,"d3", d3);
			cJSON_AddNumberToObject(root,"d5", d5);
			
			cJSON_AddStringToObject(root,"d2str",(char *)USART2_RX_BUF);
			cJSON_AddStringToObject(root,"d3str",(char *)USART3_RX_BUF);
			cJSON_AddStringToObject(root,"d5str",(char *)UART5_RX_BUF);

			strJson=cJSON_Print(root); 
			cJSON_Delete(root); 
			
			head[2] = strlen(strJson);
			strncpy(strSend,(char *)head,3);
			strncpy(strSend+3,strJson,strlen(strJson));
			printf("%s\n",strSend); 
			
			myfree(strJson);
			delay_ms(500);
		}
		
		
		
	}	 
}














