#include "main.h"


/***************************************************
*������    ��Adc_Voltage_Config
*��������  ��ADC��ȡ��ص�ѹ��ʼ��
*��������  ����
*��������ֵ����
*��������  ����Դ��ѹ��� PC0 ----- ADC1_IN10
****************************************************/
void Adc_Voltage_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_InitTypeDef ADC_InitStruct = {0};
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1,ENABLE); 
	//PC0����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;		//ģ������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOC,&GPIO_InitStruct); 
	//ADC��Ƶ //6��Ƶ 12MHZ
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	//ADC1����
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;		//����ģʽ
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		 //ADC �����Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;			// ADC1 �� ADC2 �����ڶ���ģʽ
	ADC_InitStruct.ADC_NbrOfChannel = 1;					//һ��ͨ��
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;				//��ɨ��
	ADC_Init(ADC1,&ADC_InitStruct);
	//ʹ��ADC1
	ADC_Cmd(ADC1,ENABLE);
	
	//ADCУ׼
	ADC_ResetCalibration(ADC1);
	//��ȡADCУ׼�����״̬
	while(ADC_GetResetCalibrationStatus(ADC1));
	//����У׼
	ADC_StartCalibration(ADC1);
	//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
}


/***************************************************
*������    ��Adc1_GetValue
*��������  ��ADC��ȡ��ص�ѹת������������
*��������  ��u8 channel
*��������ֵ������ת��������������
*��������  ��
****************************************************/
u16 Adc1_GetValue(u8 channel)
{
	int i = 0;
	int j = 0;
	u16 temp = 0;
	u16 buff[10];
	//ѡ��ͨ���Ͳ�������
	ADC_RegularChannelConfig(ADC1,channel,1,ADC_SampleTime_239Cycles5);
	
	for(i = 0;i < 10;i++)
	{
		//�������
		ADC_SoftwareStartConvCmd(ADC1,ENABLE);
		//�ȴ�ת�����
		while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
		buff[i] =  ADC_GetConversionValue(ADC1);
//		temp =  ADC_GetConversionValue(ADC1);
	}
	//����˲�  ȥ�����ֵ ȥ����Сֵ
	//1.�ɱƶ���-----ð������
	for(i = 0;i < 10 - 1;i++)
	{
		for(j = 0;j < 10 - i - 1;j++)
		{
			if(buff[j] > buff[j + 1])
			{
				temp = buff[j];
				buff[j] = buff[j + 1];
				buff[j + 1] = temp;
			}
		}
	}
	temp = 0;
	for(i = 1;i < 9;i++)
	{
		temp += buff[i];
	}
	temp /= 8;
	return temp;
	
}

/***************************************************
*������    ��Adc1_GetValue
*��������  ��ADC��ص�ѹ������
*��������  ����
*��������ֵ����
*��������  �������ת����ĵ�ص�ѹ��С
****************************************************/
void ADC_Get(void)
{
	u16 temp = 0;
	float V_Arm = 0;
	//��ȡGZ����������ӡ
	temp = Adc1_GetValue(10);
//	V_Arm = (1.4 / 4096) * temp * 6 + 6;
//printf("Varm��%.1f\r\n",V_Arm);
//	printf("��ѹ��������%d\r\n",temp);
// 	printf("V:%f\r\n",(1.0+ temp)/207);	
//	printf("V:%f\r\n",(1.4 / 4096) * temp * 6 + 4);
	printf("V:%f\r\n",(3.3*temp) /4096 *6);
	if(V_Arm < 6.7)
	{
		MOTOR_POWER_OFF;
	}
}

