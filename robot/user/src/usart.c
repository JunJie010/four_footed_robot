#include "main.h"


/***************************************************
*������    ��usart1_init
*��������  ��USART1��ʼ��
*��������  ��u32 bps  ������
*��������ֵ����
*��������  ��USART1_TXD --- PA9 --- �����������
						 USART1_RXD --- PA10 --- ��������ģʽ
****************************************************/
void usart1_init(u32 bps)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//��USART1ʱ��
	
	GPIO_InitTypeDef GPIO_InitStruct={0};//����ṹ�������
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//����Ϊ�����������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;//���õ�9�Źܽ�
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�Ϊ50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;//���ùܽ�
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	USART_InitTypeDef USART_InitStruct={0};
	USART_InitStruct.USART_BaudRate = bps;//������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ʹ�ܷ������ͽ�����
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλΪ1bit
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8bit
	USART_Init(USART1,&USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);//ʹ�ܿ����ж�
	//NVIC
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;//�����ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//ռ��
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//��Ӧ
	
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1,ENABLE);//ʹ��USART1
}



/***************************************************
*������    ��USART1_IRQHandler
*��������  ���жϷ�����
*��������  ����
*��������ֵ����
*��������  ��
****************************************************/
USART1STRUCT usart1_rec={0};
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		usart1_rec.data[usart1_rec.len++] = USART_ReceiveData(USART1);
		
	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		USART1->DR;
		USART1->SR;
		
		usart1_rec.data[usart1_rec.len] = '\0';
		usart1_rec.len = 0;
		usart1_rec.flag = 1;
		
		printf("%s",usart1_rec.data);
	}
}



/***************************************************
*������    ��usart1_send_byte
*��������  ��USART1����1�ֽں���
*��������  ��u8 data
*��������ֵ����
*��������  ��USART1_TXD --- PA9 --- �����������
             USART1_RXD --- PA10 --- ��������ģʽ
****************************************************/
void usart1_send_byte(u8 data)
{
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
	USART_SendData(USART1,data);

}



/***********************
�������ܣ�printf�ض���
�βΣ�u8 data
����ֵ��void
************************/
int fputc(int c, FILE * stream)
{
	usart1_send_byte(c);
	return c;	
}



