#include "main.h"



/*******************************************
��������led_init
�������ܣ�LED��ʼ��
�βΣ�void
����ֵ��void
����˵����
LED1 --- PA8 --- ͨ���������
�����ƣ�
1��	��ʱ�ӣ�GPIOA��
2��	��ʼ��PA8
3��	LEDĬ����״̬ --- �͵�ƽ����
4��	���
5����ʱ����
**********************************************/
void led_init(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};//����ṹ�����
	//GPIOA8����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		//�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;				// ѡ�йܽ� 8
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// ���������� 50MHz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//Ĭ����
	LED1_OFF;
}





