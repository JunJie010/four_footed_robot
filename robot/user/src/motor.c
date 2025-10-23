#include "main.h"


/***************************************************
*������    ��MOTOR_Power
*��������  ��������Ƶ�Դ��·
*��������  ��u32 bound    ������
*��������ֵ����
*��������  ��	MOTOR_POWER --- PA15 --- ͨ���������
							PA15����ߵ�ƽ���ͨ��
							PA15����͵�ƽ����ϵ�
****************************************************/
//������Ƶ�Դ��· PA15---��ӳ�� �͵�ƽʧ��
void MOTOR_Power(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	//��ӳ��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	//PA15����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct); 
	
	MOTOR_POWER_OFF;
	
//	//1����GPIOAʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	//2��PA15��ʼ��   ͨ���������
//	GPIOA->CRH &=~ (0xfu << 28);//����λ����
//	GPIOA->CRH |= 3 << 28;//��PA15����Ϊ���ģʽ������ٶ�Ϊ50Mhz
//	GPIOA->CRH &=~ (3u << 30);//����PA15����Ϊͨ���������
//	
//	//3��Ĭ�ϳ�ʼ����ɺ󣬶���ϵ�
//	MOTOR_POWER_OFF;
	
}


/****************************************************************
*������    ��ArmCtrl_Config
*��������  �������ʼ������
*��������  ����
*��������ֵ����
*��������  ��	��е�۶����ʼ������   ͨ�ö�ʱ�� ���PWM  ռ�ձ�
							arm0---PB8---TIM4_CH3-----��ǰ��
							arm1---PB9---TIM4_CH4-----��ǰ��
							arm2---PB6---TIM4_CH1-----�Һ���
							arm3---PB7---TIM4_CH2-----�Һ���
							arm4---PA6---TIM3_CH1-----��ǰ��
							arm5---PA7---TIM3_CH2-----�޷�Ӧ
							arm6---PA0---TIM5_CH1-----�����
							arm7---PA1---TIM5_CH2-----�����
****************************************************************/
Arm_Countinfo armflag;
void ArmCtrl_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	//GPIO����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//��ʱ����������
	TIM_TimeBaseStructure.TIM_Period = 20000;                				//�Զ���װ������ֵ(1+TIM_Period)
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;             				//Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				  					//�ָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 		//���ϼ���
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//��ʱ�����PWM����
		/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              	//���PWM����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      	//�ߵ�ƽ��Ч���ǵ͵�ƽ��Ч
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              	//���PWM����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      	//�ߵ�ƽ��Ч���ǵ͵�ƽ��Ч
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              	//���PWM����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      	//�ߵ�ƽ��Ч���ǵ͵�ƽ��Ч
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM5, ENABLE);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	//��ʱ��ʹ��
	TIM_Cmd(TIM5, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	delay_ms(500);
}



//���ת�����
#define PWM_MIX (500)
#define PWM_MAX (2500)
#define ANGLE_MAX (180.0f)
#define ANGLE_PWM ((PWM_MAX - PWM_MIX)/ANGLE_MAX)

/**
  * @brief  ����Ƕ�ֵ���ƶ�Ӧ�Ķ���Ƕ�
  * @param  �����
  * @param  �Ƕ� 0~179
  * @retval None
  */
void ArmCtrl_Angle (ArmDef ArmNum, float Angle)
{
	uint16_t Compare;
	Compare = PWM_MIX + ANGLE_PWM*Angle - 1;
	switch (ArmNum)
	{
		case ARM_RIGHT_BEFORE_L:			
			TIM_SetCompare3(TIM4, Compare);		//*
			break;
		case ARM_RIGHT_BEFORE_H:
			TIM_SetCompare4(TIM4, Compare);		//*
			break;
		case ARM_RIGHT_AFTER_H:
			TIM_SetCompare1(TIM4, Compare);		//*
			break;
		case ARM_RIGHT_AFTER_L:
			TIM_SetCompare2(TIM4, Compare);		//*
			break;
		case ARM_LEFT_BEFORE_L:
			TIM_SetCompare1(TIM3, Compare);		//*
			break;
		case ARM_LEFT_BEFORE_H:					
			TIM_SetCompare2(TIM3, Compare);		//*
			break;
		case ARM_LEFT_AFTER_H:
			TIM_SetCompare1(TIM5, Compare);		//*
			break;
		case ARM_LEFT_AFTER_L:
			TIM_SetCompare2(TIM5, Compare);		//*
			break;
		default:
			break;
	}
}


//�����������
//��ʼ����
void Arm_Zero(void)
{
	for(int i = 0;i < 8;i++)
	{
		ArmCtrl_Angle(i,90);
	}
}
//��������
void Arm_Li(void)
{
	int ap = 30;
	int hi = 0;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};		//��������
	for(int i = 0;i < 8;i++)
	{
		ArmCtrl_Angle(i,position[i]);
		delay_ms(500);
	}
}
//��Ծ-----OK
void Arm_Jump(void) 
{
	int ap = 25;
	int hi = 25;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};		//����ƽ��
	//���ʹ��
	if(ARM_JUMP)			//�ж��ǵ�ǰ����
	{
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
		  ArmCtrl_Angle(i, position[i]);
		}
		//��
		armflag.action_push_up = true;
		armflag.action_jump_down = false;
		armflag.arm_time5 = 0;
		while(armflag.action_push_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 40)
			{
				armflag.arm_time5 = 0;
				armflag.action_push_up = false;
				armflag.action_jump_down = true;
			}
		}
		//��
		while(armflag.action_jump_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,130 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,130 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,50 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,50 + armflag.arm_time5);
			if(armflag.arm_time5 == 40)
			{
				armflag.arm_time5 = 0;
				armflag.action_push_up = true;
				armflag.action_jump_down = false;
			}
		}		
	}
}

//���Գ�-----OK
void Arm_Push(void)
{
	int ap = 30;
	int hi = 50;
	int position[] = { 90 + ap, 90, 90 + hi, 90, 90 - ap, 90, 90 - hi, 90};		//���㲢£
	if((ARM_PUSHUP))			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		armflag.action_push_up = true;
		armflag.action_push_down = false;
		//���ʹ��
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
			ArmCtrl_Angle(i, position[i]);
		}
		//����ʱ������
		armflag.arm_time10 = 0;
		//��
		while(armflag.action_push_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,150 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,30 + armflag.arm_time10);
//			printf("temp1 = %d\r\n", armflag.arm_time10);
			if(armflag.arm_time10 == 60)
			{
				armflag.arm_time10 = 0;
				armflag.action_push_up = false;
				armflag.action_push_down = true;
				break;
			}
		}
		//��
		while(armflag.action_push_down == true)
		{
//			printf("temp2 = %d\r\n", armflag.arm_time10);
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time10);	
			if(armflag.arm_time10 == 60)
			{
				armflag.arm_time10 = 0;
				armflag.action_push_up =  true;
				armflag.action_push_down = false;
				break;
			}
		}
	}	
}

//���к�-----OK
void Arm_Hello(void)
{
	int ap = 30;
	int hi = 50;
	int position[] = { 10, 120, 90 - hi, 90 + ap, 90 - ap, 90, 90 + hi, 100};			//��ǰ̧�� ����½�
	if( ARM_SAYHELLO)			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
			ArmCtrl_Angle(i,position[i]);
		}
		armflag.action_sayhello_up = true;
		armflag.action_sayhello_down = false;
		armflag.arm_time5 = 0;			//����ʱ������
		while(armflag.action_sayhello_up == true)
		{
			//��ǰ��ǰ�Ӷ�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,120 - armflag.arm_time5);
			//���°���
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,10 + armflag.arm_time5);
			if(armflag.arm_time5 == 60)
			{
				armflag.arm_time5 = 0;
				armflag.action_sayhello_up = false;
				armflag.action_sayhello_down = true;
				break;
			}
		}
		while(armflag.action_sayhello_down == true)
		{
			//��ǰ���Ӷ�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,60 + armflag.arm_time5);
			//���ϰ���
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,70 - armflag.arm_time5);
			if(armflag.arm_time5 == 60)
			{
				armflag.arm_time5 = 0;
				armflag.action_sayhello_up = true;
				armflag.action_sayhello_down = false;
				break;
			}
		}
	}
}

//Ť��
void Arm_Twisting(void)
{
	int ap = 30;
	int hi = 0;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 + ap, 90 - hi, 90 + hi, 90 + ap};			//��ǰ̧�� ����½�
	if(ARM_TEIST)			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		//��ʼ����
		for (int i = 0; i < 8; i++) 			
		{
			ArmCtrl_Angle(i,position[i]);
		}
		armflag.action_twisting_up = true;
		armflag.action_twisting_down = false;
		armflag.arm_time10 = 0;			//����ʱ������
		while(armflag.action_twisting_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,120 - 2 * armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,120 - 2 * armflag.arm_time10);
			if(armflag.arm_time10 == 20)
			{
				armflag.arm_time10 = 0;
				armflag.action_twisting_up = false;
				armflag.action_twisting_down = true;
			}
		}
		while(armflag.action_twisting_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,120 - 2 * armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,120 - 2 * armflag.arm_time10);
			if(armflag.arm_time10 == 20)
			{
				armflag.arm_time10 = 0;
				armflag.action_twisting_up = true;
				armflag.action_twisting_down = false;
			}
		}
		while(armflag.action_twisting_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + 2 * armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 + armflag.arm_time10);
			if(armflag.arm_time10 == 20)
			{
				armflag.arm_time10 = 0;
				armflag.action_twisting_up = false;
				armflag.action_twisting_down = true;
			}
		}
		while(armflag.action_twisting_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + 2 * armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 + armflag.arm_time10);
			if(armflag.arm_time10 == 20)
			{
				armflag.arm_time10 = 0;
				armflag.action_twisting_up = true;
				armflag.action_twisting_down = false;
			}
		}
	}
}
//�䶯------OK
void Arm_Peristalsis(void)
{
	int ap = 25;
	int hi = 0;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};			//��ǰ̧�� ����½�
	if(ARM_PERISTALSIS)			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
			ArmCtrl_Angle(i,position[i]);
		}
		armflag.action_Peristalsis_up = true;
		armflag.action_Peristalsis_down = false;
		armflag.arm_time10 = 0;			//����ʱ������
		while(armflag.action_Peristalsis_up == true)
		{
			//�ϲ�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,90 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,90 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,90 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,90 + armflag.arm_time10);
			delay_ms(5);
			//�²�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,125 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,125 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,55 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,55 + armflag.arm_time10);
			if(armflag.arm_time10 == 35)
			{
				armflag.arm_time10 = 0;			//����ʱ������
				armflag.action_Peristalsis_up = false;
				armflag.action_Peristalsis_down = true;
			}
		}
		while(armflag.action_Peristalsis_down == true)
		{
			//�ϲ�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,55 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,55 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,125 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,125 - armflag.arm_time10);
			delay_ms(5);
			//�²�
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time10);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 - armflag.arm_time10);
			if(armflag.arm_time10 == 35)
			{
				armflag.arm_time10 = 0;			//����ʱ������
				armflag.action_Peristalsis_up = true;
				armflag.action_Peristalsis_down = false;
			}
		}
	}
}
//��ת
void Arm_Left(void)
{
	int ap = 30;
	int hi = 0;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};			
	if(ARM_TURNLEFT)			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
			ArmCtrl_Angle(i,position[i]);
		}
		armflag.action_left_up = true;
		armflag.action_left_down = false;
		armflag.arm_time5 = 0;			//����ʱ������
		//0 7̧��
		while(armflag.action_left_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,60 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = false;
				armflag.action_left_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//1 6˳ʱ��
		while(armflag.action_left_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,90 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = true;
				armflag.action_left_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//0 7����
		while(armflag.action_left_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = false;
				armflag.action_left_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//3 4̧��
		while(armflag.action_left_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,60 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = true;
				armflag.action_left_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//2 5��ʱ��
		while(armflag.action_left_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,90 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 20)
			{
				armflag.action_left_up = false;
				armflag.action_left_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//1 6��ʱ��
		while(armflag.action_left_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,60 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,60 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = true;
				armflag.action_left_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//3 4����
		while(armflag.action_left_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_left_up = false;
				armflag.action_left_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//2 5˳ʱ��
		while(armflag.action_left_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,70 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,70 + armflag.arm_time5);
			if(armflag.arm_time5 == 20)
			{
				armflag.action_left_up = true;
				armflag.action_left_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
	}	
}

//��ת
void Arm_Right(void)
{
	int ap = 30;
	int hi = 0;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};			//��ǰ̧�� ����½�
	if(ARM_TURNRIGHT)			//�ж��ǵ�ǰ����
	{
		//���ʹ��
		MOTOR_POWER_ON;
		for (int i = 0; i < 8; i++) 
		{
			ArmCtrl_Angle(i,position[i]);
		}
		armflag.action_right_up = true;
		armflag.action_right_down = false;
		armflag.arm_time5 = 0;			//����ʱ������
		//0 7̧��
		while(armflag.action_right_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,60 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = false;
				armflag.action_right_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//1 6��ʱ��
		while(armflag.action_right_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,90 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = true;
				armflag.action_right_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//0 7����
		while(armflag.action_right_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = false;
				armflag.action_right_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//3 4̧��
		while(armflag.action_right_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,60 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up =true;
				armflag.action_right_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//2 5˳ʱ��
		while(armflag.action_right_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,90 + armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = false;
				armflag.action_right_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//1 6˳ʱ��
		while(armflag.action_right_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_AFTER_H,120 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = true;
				armflag.action_right_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//3 4����
		while(armflag.action_right_up == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = false;
				armflag.action_right_down = true;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
		//2 5��ʱ��
		while(armflag.action_right_down == true)
		{
			ArmCtrl_Angle(ARM_RIGHT_AFTER_H,120 - armflag.arm_time5);
			ArmCtrl_Angle(ARM_LEFT_BEFORE_H,120 - armflag.arm_time5);
			if(armflag.arm_time5 == 30)
			{
				armflag.action_right_up = true;
				armflag.action_right_down = false;
				armflag.arm_time5 = 0;			//����ʱ������
			}
		}
	}
}






//��������
void Arm_ss(void)
{
	int ap = 60;
	int hi = 30;
	int position[] = { 90 + ap, 90 + hi, 90 - hi, 90 + ap, 90 - ap, 90 - hi, 90 + hi, 90 - ap};		//��������
	for(int i = 0;i < 8;i++)
	{
		ArmCtrl_Angle(i,position[i]);
		delay_ms(30);
	}
}




//ǰ��
void Arm_Forward(void)
{
	//              0   1   2   3   4   5    6    7
	u8 position[]={120,120,60, 90, 90, 90 , 90 , 60};
	
	for(u8 i=0;i<8;i++)
	{
		ArmCtrl_Angle(i,position[i]);
	}
	armflag.arm_time5 = 0;
	armflag.action_forward_up = 1;
	armflag.action_forward_down = 0;
	//��һ������    0�Ŷ��̧��    3�Ŷ������     4�Ŷ������      7�Ŷ��̧��
	while(armflag.action_forward_up == 1)
	{
		ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,120 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_RIGHT_AFTER_L,90 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_BEFORE_L,90 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_AFTER_L,60 + armflag.arm_time5);
		if(armflag.arm_time5 == 30)
		{
			armflag.arm_time5 = 0;
			armflag.action_forward_up = 0;
			armflag.action_forward_down = 1;
		}
	}
	//�ڶ�������  �ϱ۲���     1�š�5����ʱ��    2�š�6��˳ʱ��
	while(armflag.action_forward_down == 1)
	{
		ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,120 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_RIGHT_AFTER_H,60 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_BEFORE_H,90 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_AFTER_H,90 + armflag.arm_time5);
		if(armflag.arm_time5 == 30)
		{
			armflag.arm_time5 = 0;
			armflag.action_forward_up = 1;
			armflag.action_forward_down = 0;
		}
	}
	//����������    0�Ŷ������    3�Ŷ��̧��     4�Ŷ��̧��      7�Ŷ������
	while(armflag.action_forward_up == 1)
	{
		ArmCtrl_Angle(ARM_RIGHT_BEFORE_L,90 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_RIGHT_AFTER_L,120 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_BEFORE_L,60 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_AFTER_L,90 - armflag.arm_time5);
		if(armflag.arm_time5 == 30)
		{
			armflag.arm_time5 = 0;
			armflag.action_forward_up = 0;
			armflag.action_forward_down = 1;
		}
	}
	//���ĸ�����  �ϱ۲���     1�š�5��˳ʱ��    2�š�6����ʱ��
	while(armflag.action_forward_down == 1)
	{
		ArmCtrl_Angle(ARM_RIGHT_BEFORE_H,90 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_RIGHT_AFTER_H,90 - armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_BEFORE_H,60 + armflag.arm_time5);
		ArmCtrl_Angle(ARM_LEFT_AFTER_H,120 - armflag.arm_time5);
		if(armflag.arm_time5 == 30)
		{
			armflag.arm_time5 = 0;
			armflag.action_forward_up = 1;
			armflag.action_forward_down = 0;
		}
	}	
}

