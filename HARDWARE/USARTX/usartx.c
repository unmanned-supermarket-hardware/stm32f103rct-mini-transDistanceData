
#include "usartx.h"
//----------------------------------------------------------����2-------------------------------------------------------------//
void uart2_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //ʹ��PORTA��ʱ��  
	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���ʱ�� 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO״̬����
	GPIOA->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART2->CR1|=1<<8;    //PE�ж�ʹ��
	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//��2��������ȼ� 
}
/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//���յ�����
	{	      
		u8 temp;
		char strTemp[64];
		temp =USART2->DR;
		sprintf(strTemp,"����2�յ�����:\t%c\r\n",temp);
		usart1_sendString(strTemp,strlen(strTemp));
		usart2_sendString(strTemp,strlen(strTemp));
		usart3_sendString(strTemp,strlen(strTemp)); 	
		uart5_sendString(strTemp,strlen(strTemp)); 
   }
return 0;	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ֽ�
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart2����һ���ַ���
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

//----------------------------------------------------------����3-------------------------------------------------------------//
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart3����һ���ֽ�
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART3->SR&0x40)==0);	
}
/**************************ʵ�ֺ���**********************************************
*��    ��:		usart3����һ���ַ���
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
�������ܣ�����3��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	

	RCC->APB2ENR|=1<<0;    //��������ʱ��
	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO״̬����
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //������ӳ��

	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//��2��������ȼ� 
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/

#define dD 1
#define dColon 2
#define dComma 3

#define oO 1
#define oEnd 2

int USART3_IRQHandler(void)
{	
	if(USART3->SR&(1<<5))//���յ�����
	{	      
			u8 temp;
			char strTemp[64];
			temp =USART2->DR;
			sprintf(strTemp,"����3�յ�����:\t%c\r\n",temp);
			usart1_sendString(strTemp,strlen(strTemp));
			usart2_sendString(strTemp,strlen(strTemp));
			usart3_sendString(strTemp,strlen(strTemp)); 	
			uart5_sendString(strTemp,strlen(strTemp)); 
		}
			
   
return 0;	
}

//----------------------------------------------------------����5-------------------------------------------------------------//
/**************************ʵ�ֺ���**********************************************
*��    ��:		uart5����һ���ֽ�
*********************************************************************************/
void uart5_send(u8 data)
{
	UART5->DR = data;
	while((UART5->SR&0x40)==0);	
}


/**************************ʵ�ֺ���**********************************************
*��    ��:		uart5����һ���ַ���
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
/**************************ʵ�ֺ���**********************************************
*��    ��:		uart5��ʼ��
*********************************************************************************/
//TX PC12
//RX PD2
void uart5_init(u32 pclk1,u32 bound)	
//���� 2��3��4��5 ��pclk1 36m  ���ø�ʽuart_init5 (36,9600);    //9600������pclk1 =36m
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 

	RCC->APB2ENR|=1<<5;    //ʹ��PORTdʱ��
	RCC->APB2ENR|=1<<4;    //ʹ��PORTcʱ��  

	RCC->APB1ENR|=1<<20;  //ʹ�ܴ���5ʱ�� 	  2-��17    5-��20    �Ƴ�4-��19
	
			  
	RCC->APB1RSTR|=1<<20;   //��λ����4
	RCC->APB1RSTR&=~(1<<20);//ֹͣ��λ
		   	   
	//����������
 	UART5->BRR=mantissa; // ����������	 
	UART5->CR1|=0X200C;  //1λֹͣ,��У��λ.
   
	//IO״̬���� RX PD2 ���ս����� ����   11-8:1000
	GPIOD->CRL&=~(0X00000007<<8);//IO״̬���� ���ս����� ����     
	GPIOD->CRL|=0x00000001<<11;//IO״̬���� ���ս����� ���� 
	//��������
	GPIOD->ODR|=1<<2;	  //PD2����
	
  
	//IO״̬���� ���ͽ�TX PC12 ���ã��������������,���11     19-16: 1011
	GPIOC->CRH&=~(0X00000001<<18);         // 
	GPIOC->CRH|=0X0000000B<<16;              //
	 
	
	UART5->CR1|=1<<8;    //PE�ж�ʹ��   ���8��5����ÿ�����ڶ�һ��
	UART5->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,UART5_IRQn,2);//��ռ���ȼ���3 ��Ӧ���ȼ���3 ��2��������ȼ� 
}


void UART5_IRQHandler(void)    // ע��Ҫ��UART5 ����USART5
 //	 	 UART5_IRQHandler  ע�⺯���� ��Ҫд��
{	
	if(UART5->SR&(1<<5))//���յ�����
	{
			u8 temp;
			char strTemp[64];
			temp =USART2->DR;
			sprintf(strTemp,"����5�յ�����:\t%c\r\n",temp);
			usart1_sendString(strTemp,strlen(strTemp));
			usart2_sendString(strTemp,strlen(strTemp));
			usart3_sendString(strTemp,strlen(strTemp)); 	
			uart5_sendString(strTemp,strlen(strTemp)); 
	}						
}


