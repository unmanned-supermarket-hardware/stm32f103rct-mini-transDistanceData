
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
	USART2->SR;
}
/**************************************************************************
�������ܣ�����2�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
u8 USART2_RX_BUF[64]; //���յ�������
u16 USART2_RX_STA=0; 
u8 USART2_COUNT = 0;
double d2 =0;
char d2Str[5];
u8 USART2_RX_FLAG = 0;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

int USART2_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(USART2->SR&(1<<5))//���յ�����
	{	      
			res =USART2->DR;
		USART2_RX_FLAG = 1;
		//usart2_send(res);
			if((USART2_RX_STA&0x8000)==0)//����δ���
			{
				if(USART2_RX_STA&0x4000)//���յ���0x0d
				{
					if(res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
					else 
					{
						USART2_RX_STA|=0x8000;	//���������
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						//usart1_sendString((char *)USART2_RX_BUF,USART2_COUNT+1);
						//-------------------
						USART2_RX_BUF[(USART2_RX_STA&0X3FFF)-2] = '\0';
						strncpy(d2Str,(char *)(USART2_RX_BUF+2),6);
						sprintf(strTemp,"%s\r\n",d2Str);
						
						//usart1_sendString(strTemp,strlen(strTemp));
						if(USART2_RX_BUF[0] =='D')
							d2 = atof(d2Str);
						else
							d2 =-1;
						//-------------------

						USART2_RX_STA = 0;
						USART2_COUNT = 0;
					}
					
				}else //��û�յ�0X0D
				{	
					if(res==0x0d)
					{
						USART2_RX_STA|=0x4000;
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						USART2_RX_STA++;
						USART2_COUNT ++;
					}
					else
					{
						USART2_RX_BUF[USART2_RX_STA&0X3FFF]=res;
						USART2_RX_STA++;
						USART2_COUNT ++;
						if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
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
	
	USART2->CR1 &=~(1<<2);  //���ν���
	USART2_RX_STA =0;    //������յ�һ��ͷ�����������
	USART2_COUNT =0;
	
	for(i=0;i<len;i++)
	{
		USART2->DR = data[i];
		while((USART2->SR&0x40)==0);	
	}
	USART2->CR1 |=1<<2;   //���¿�������
}

//----------------------------------------------------------����3-------------------------------------------------------------//

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
	

	RCC->APB2ENR|=1<<0;    //��������ʱ��  ��һλ�Ǳ����ģ���֪��Ҫ������
	RCC->APB2ENR|=1<<3;   //ʹ��PORTB��ʱ��   ����λ����a 2    d 5
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 	 ����λ���� 2-��17    5-��20    �Ƴ�4-��19
	
	GPIOB->CRH&=0XFFFF00FF;   //
	GPIOB->CRH|=0X00008B00;//IO״̬����
	//GPIOB->ODR|=1<<11;	 


	RCC->APB1RSTR|=1<<18;   //��λ����3
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);//��2��������ȼ� 
	
	USART3->SR;
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
u8 USART3_RX_BUF[64]; //���յ�������
u16 USART3_RX_STA=0; 
u8 USART3_COUNT = 0;
char d3Str[5];
double d3=0;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

int USART3_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(USART3->SR&(1<<5))//���յ�����
	{	      
			res =USART3->DR;
			if((USART3_RX_STA&0x8000)==0)//����δ���
			{
				if(USART3_RX_STA&0x4000)//���յ���0x0d
				{
					if(res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
					else 
					{
						USART3_RX_STA|=0x8000;	//���������
						USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res;
						//usart1_sendString((char *)USART2_RX_BUF,USART2_COUNT+1);
						//-------------------
						USART3_RX_BUF[(USART3_RX_STA&0X3FFF)-2] = '\0';
						strncpy(d3Str,(char *)(USART3_RX_BUF+2),6);
						sprintf(strTemp,"%s",d3Str);
						
						//usart1_sendString(strTemp,strlen(strTemp));
						if(USART3_RX_BUF[0] =='D')
							d3 = atof(d3Str);
						else 
							d3 = -1;
						//-------------------

						USART3_RX_STA = 0;
						USART3_COUNT = 0;
						
					}
					
				}else //��û�յ�0X0D
				{	
					if(res==0x0d)
					{
						USART3_RX_STA|=0x4000;
						USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res;
						USART3_RX_STA++;
						USART3_COUNT ++;
					}
					else
					{
						USART3_RX_BUF[USART3_RX_STA&0X3FFF]=res;
						USART3_RX_STA++;
						USART3_COUNT ++;
						if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}
	}
return 0;	
}
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
	USART3->CR1 &=~(1<<2);  //���ν���
	USART3_RX_STA =0;    //������յ�һ��ͷ�����������
	USART3_COUNT =0;
	for(i=0;i<len;i++)
	{
		USART3->DR = data[i];
		while((USART3->SR&0x40)==0);	
	}
	USART3->CR1 |=1<<2;   //���¿�������
}
//----------------------------------------------------------����4-------------------------------------------------------------//

/**************************************************************************
�������ܣ�����4��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart4_init(u32 pclk2,u32 bound)
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
	RCC->APB1ENR|=1<<19;  //ʹ�ܴ���ʱ��   //ʹ�ܴ���5ʱ�� 	  2-��17    5-��20    �Ƴ�4-��19
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO״̬����
	//GPIOC->ODR|=1<<11;	 


	RCC->APB1RSTR|=1<<19;   //��λ����4
	RCC->APB1RSTR&=~(1<<19);//ֹͣ��λ	   	   
	//����������
 	UART4->BRR=mantissa; // ����������	 
	UART4->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	UART4->CR1|=1<<8;    //PE�ж�ʹ��
	UART4->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,UART4_IRQn,2);//��2��������ȼ� 
	UART4->SR;
}

/**************************************************************************
�������ܣ�����4�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
u8 UART4_RX_BUF[64]; //���յ�������
u16 UART4_RX_STA=0; 
u8 UART4_COUNT = 0;
char d4Str[5];
double d4 = 0;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

int UART4_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(UART4->SR&(1<<5))//���յ�����
	{	      
			res =UART4->DR;
			if((UART4_RX_STA&0x8000)==0)//����δ���
			{
				if(UART4_RX_STA&0x4000)//���յ���0x0d
				{
					if(res!=0x0a)UART4_RX_STA=0;//���մ���,���¿�ʼ
					else 
					{
						UART4_RX_STA|=0x8000;	//���������
						UART4_RX_BUF[UART4_RX_STA&0X3FFF]=res;
						//usart1_sendString((char *)USART2_RX_BUF,USART2_COUNT+1);
						//-------------------
						UART4_RX_BUF[(UART4_RX_STA&0X3FFF)-2] = '\0';
						strncpy(d4Str,(char *)(UART4_RX_BUF+2),6);
						sprintf(strTemp,"%s",d4Str);
						
						//usart1_sendString(strTemp,strlen(strTemp));
						if(UART4_RX_BUF[0] =='D')
							d4 = atof(d4Str);
						else
							d4 = -1;
						//-------------------

						UART4_RX_STA = 0;
						UART4_COUNT = 0;
					}
					
				}else //��û�յ�0X0D
				{	
					if(res==0x0d)
					{
						UART4_RX_STA|=0x4000;
						UART4_RX_BUF[UART4_RX_STA&0X3FFF]=res;
						UART4_RX_STA++;
						UART4_COUNT ++;
					}
					else
					{
						UART4_RX_BUF[UART4_RX_STA&0X3FFF]=res;
						UART4_RX_STA++;
						UART4_COUNT ++;
						if(UART4_RX_STA>(USART_REC_LEN-1))UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
					}		 
				}
			}
	}
return 0;	
}

/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ֽ�
*********************************************************************************/
void uart4_send(u8 data)
{
	UART4->DR = data;
	while((UART4->SR&0x40)==0);	
}
/**************************ʵ�ֺ���**********************************************
*��    ��:		uart4����һ���ַ���
*********************************************************************************/
void uart4_sendString(char *data,u8 len)
{
	int i=0;
	UART4->CR1 &=~(1<<2);  //���ν���
	UART4_RX_STA =0;    //������յ�һ��ͷ�����������
	UART4_COUNT =0;
	
	
	for(i=0;i<len;i++)
	{
		UART4->DR = data[i];
		while((UART4->SR&0x40)==0);	
	}
	UART4->CR1 |=1<<2;   //���¿�������
}
//----------------------------------------------------------����5-------------------------------------------------------------//

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
	UART5->SR;
}


/**************************************************************************
�������ܣ�����5�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
u8 UART5_RX_BUF[64]; //���յ�������
u16 UART5_RX_STA=0; 
u8 UART5_COUNT = 0;
char d5Str[5];
double d5 = 0;
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ

int UART5_IRQHandler(void)
{	
	u8 res;
	char strTemp[64];
	if(UART5->SR&(1<<5))//���յ�����
	{	      
			res =UART5->DR;
			if((UART5_RX_STA&0x8000)==0)//����δ���
			{
				if(UART5_RX_STA&0x4000)//���յ���0x0d
				{
					if(res!=0x0a)UART5_RX_STA=0;//���մ���,���¿�ʼ
					else 
					{
						UART5_RX_STA|=0x8000;	//���������
						UART5_RX_BUF[UART5_RX_STA&0X3FFF]=res;
						//usart1_sendString((char *)USART2_RX_BUF,USART2_COUNT+1);
						//-------------------
						UART5_RX_BUF[(UART5_RX_STA&0X3FFF)-2] = '\0';
						strncpy(d5Str,(char *)(UART5_RX_BUF+2),6);
						sprintf(strTemp,"%s",d5Str);
						
						//usart1_sendString(strTemp,strlen(strTemp));
						if(UART5_RX_BUF[0] =='D')
							d5 = atof(d5Str);
						else
							d5 = -1;
						//-------------------

						UART5_RX_STA = 0;
						UART5_COUNT = 0;
					}
					
				}else //��û�յ�0X0D
				{	
					if(res==0x0d)
					{
						UART5_RX_STA|=0x4000;
						UART5_RX_BUF[UART5_RX_STA&0X3FFF]=res;
						UART5_RX_STA++;
						UART5_COUNT ++;
					}
					else
					{
						UART5_RX_BUF[UART5_RX_STA&0X3FFF]=res;
						UART5_RX_STA++;
						UART5_COUNT ++;
						if(UART5_RX_STA>(USART_REC_LEN-1))UART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	 
					}		 
				}
			}
	}
return 0;	
}

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
	UART5->CR1 &=~(1<<2);  //���ν���
	UART5_RX_STA =0;    //������յ�һ��ͷ�����������
	UART5_COUNT =0;
	for(i=0;i<len;i++)
	{
		UART5->DR = data[i];
		while((UART5->SR&0x40)==0);
	}
	UART5->CR1 |=1<<2;   //���¿�������
}

