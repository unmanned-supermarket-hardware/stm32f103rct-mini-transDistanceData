
#include "usartx.h"
//----------------------------------------------------------串口2-------------------------------------------------------------//
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //使能PORTA口时钟  
	RCC->APB1ENR|=1<<17;  //使能串口时钟 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO状态设置
	GPIOA->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART2->CR1|=1<<8;    //PE中断使能
	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//组2，最低优先级 
}
/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//接收到数据
	{	      
		u8 temp;
		char strTemp[64];
		temp =USART2->DR;
		sprintf(strTemp,"2:\t%c\r\n",temp);
		usart2_sendString(strTemp,strlen(strTemp));
   }
return 0;	
}


/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
/**************************实现函数**********************************************
*功    能:		usart2发送一个字符串
*********************************************************************************/
void usart2_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		USART2->DR = data[i];
		while((USART2->SR&0x40)==0);	
	}
	
}

//----------------------------------------------------------串口3-------------------------------------------------------------//
/**************************实现函数**********************************************
*功    能:		usart3发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************实现函数**********************************************
*功    能:		usart3发送一个字符串
*********************************************************************************/
void usart3_sendString(char *data,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		USART3->DR = data[i];
		while((USART3->SR&0x40)==0);	
	}
	
}
/**************************************************************************
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //开启辅助时钟
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO状态设置
	GPIOC->ODR|=1<<10;	 
	AFIO->MAPR|=1<<4;      //部分重映射

	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//组2，最低优先级 
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/

#define dD 1
#define dColon 2
#define dComma 3

#define oO 1
#define oEnd 2

int USART3_IRQHandler(void)
{	
	if(USART3->SR&(1<<5))//接收到数据
	{	      
			u8 temp;
			char strTemp[64];
			temp =USART3->DR;
			sprintf(strTemp,"3:\t%c\r\n",temp);
			usart3_sendString(strTemp,strlen(strTemp)); 	
	}
return 0;	
}


//----------------------------------------------------------串口5-------------------------------------------------------------//
/**************************实现函数**********************************************
*功    能:		uart5发送一个字节
*********************************************************************************/
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}


/**************************实现函数**********************************************
*功    能:		uart5发送一个字符串
*********************************************************************************/
void uart5_sendString(char * data ,u8 len)
{
	int i=0;
	for(i=0;i<len;i++)
	{
		UART5->DR = data[i];
		while((UART5->SR&0x40)==0);
	}
}
/**************************实现函数**********************************************
*功    能:		uart5初始化
*********************************************************************************/
//TX PC12
//RX PD2
void uart5_init(u32 pclk1,u32 bound)	
//串口 2，3，4，5 用pclk1 36m  调用格式uart_init5 (36,9600);    //9600波特率pclk1 =36m
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<5;    //使能PORTd时钟
	RCC->APB2ENR|=1<<4;    //使能PORTc时钟  

	RCC->APB1ENR|=1<<20;  //使能串口5时钟 	  2-》17    5-》20    推出4-》19
	
			  
	RCC->APB1RSTR|=1<<20;   //复位串口4
	RCC->APB1RSTR&=~(1<<20);//停止复位
		   	   
	//波特率设置
 	UART5->BRR=mantissa; // 波特率设置	 
	UART5->CR1|=0X200C;  //1位停止,无校验位.
   
	//IO状态设置 RX PD2 接收脚输入 上拉   11-8:1000
	GPIOD->CRL&=~(0X00000007<<8);//IO状态设置 接收脚输入 上拉     
	GPIOD->CRL|=0x00000001<<11;//IO状态设置 接收脚输入 上拉 
	//输入上拉
	GPIOD->ODR|=1<<2;	  //PD2上拉
	
  
	//IO状态设置 发送脚TX PC12 复用！！！！推挽输出,输出11     19-16: 1011
	GPIOC->CRH&=~(0X00000001<<18);         // 
	GPIOC->CRH|=0X0000000B<<16;              //
	 
	
	UART5->CR1|=1<<8;    //PE中断使能   这个8和5好像每个串口都一样
	UART5->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,UART5_IRQn,2);//抢占优先级是3 响应优先级是3 组2，最低优先级 
}


void UART5_IRQHandler(void)    // 注意要用UART5 不是USART5
 //	 	 UART5_IRQHandler  注意函数名 不要写错
{	
	if(UART5->SR&(1<<5))//接收到数据
	{
			u8 temp;
			char strTemp[64];
			temp =UART5->DR;
			sprintf(strTemp,"5:%c\r\n",temp);	
			uart5_sendString(strTemp,strlen(strTemp)); 
		  
	}						
}



