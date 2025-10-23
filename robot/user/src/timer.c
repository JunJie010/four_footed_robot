#include "main.h"

/***************************************************
*������    ��tim2_init
*��������  ����ʱ��2��ʼ������
*��������  ����
*��������ֵ����
*��������  ��	ʱ�� = ��������/����Ƶ��
	            ʱ��(ms) = �������� /10000  
							�������� = 10----10����1ms 10000--1s
							����Ƶ��= ʱ������ /��Ƶϵ��
							72000000/7200
****************************************************/
void tim2_init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//����TIM2ʱ��

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; //����ṹ�����
	NVIC_InitTypeDef NVIC_InitStruct = {0}; 
	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_Period = 1000;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;  //��Ƶֵ---
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ�ܸ����ж�
	
	//NVIC������----�ں�---���ȼ�����Ϊ����ռ ��Ӧ  ��Ȼ
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;        //ѡ��ָ����TIM2�ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;          //��NVIC_IRQChannel�ж�����ж�ͨ����ʹ�ܻ���ʧ�ܡ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;//������ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;       //������Ӧ���ȼ�      
	NVIC_Init(&NVIC_InitStruct);  
	
	
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��
	TIM_ClearFlag(TIM2,TIM_FLAG_Update); //����±�־
}



/***************************************************
*������    ��TIM2_IRQHandler
*��������  ����ʱ��2�жϷ�����
*��������  ����
*��������ֵ����
*��������  ��	
****************************************************/
uint32_t OStime = 0;
void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)==1)//��ȡ��ʱ��2�Ƿ�Ϊ�����ж�
	{
		
		TIM_ClearITPendingBit(TIM2,TIM_FLAG_Update);  //����жϱ�־λ�������ֶ����
		OStime++;
		if((OStime % 10 == 0))
		{
			armflag.arm_time10++;
		}
		if(OStime % 20 == 0)
		{
			armflag.arm_time20++;
		}
		if(OStime % 5 == 0)
		{
			armflag.arm_time5++;
		}
	}
}




