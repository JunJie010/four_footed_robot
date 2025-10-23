#include "main.h"

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�������ȼ�����
	
	led_init();            //LED1��ʼ��
	usart1_init(115200);   //USART1��ʼ��
	printf("���ڳ�ʼ���ɹ�����������\r\n");
	voice_init(9600);      //����ģ�鲨����Ĭ��9600
	ArmCtrl_Config();      //�����ʼ������
	MOTOR_Power();				 //������õ�Դ��ʼ������
	Adc_Voltage_Config();  //ADC��ȡ��ص�ѹ��ʼ������
	Set_Song_Volume(30);   //����������С��0-30����
	delay_ms(500);         ////����ģ���ϵ�ʱ��
	
	
	//�����˳�ʼ������
	Voice_Play((u8 *)"00016");
	MOTOR_POWER_ON;//�������
	Arm_Li();//�����˳�ʼ������վ����
	//ESP8266��ʼ��    2.4Ghz
	if(ESP8266_Init()==0)
		if(ESP8266_ConnectWifi((u8 *)"Robot",(u8 *)"13145230")==0)                 //�ȵ㲻Ҫ��������wife�豸����   2.4GƵ��
			if(Esp8266_ConnectServer((u8 *)" 192.168.92.215",(u8 *)"8888")==0)
				Voice_Play((u8 *)"00012");
	
	printf("��ӭʹ��,���������\r\n");
	
	tim2_init();			     //��ʱ��2��ʼ��
			
			
	u8 action_flag = 0;
	//�������в���   һֱ����
	while(1)
	{
			if(strcmp((const char *)u2.buff,"Stop")==0)
		{
			action_flag = 0;
			Voice_Play((u8 *)"00001");//����ֹͣ����
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"Pushup")==0)
		{
			action_flag = 1;
			Voice_Play((u8 *)"00010");//���Ÿ��Գ�����
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"Dance")==0)
		{
			action_flag = 2;
			Voice_Play((u8 *)"00015");//��������
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"SayHello")==0)
		{
			action_flag = 3;
			Voice_Play((u8 *)"00009");//�������
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"Forward")==0)
		{
			action_flag = 4;
			Voice_Play((u8 *)"00002");//����ֱ��
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"TurnRight")==0)
		{
			action_flag = 5;
			Voice_Play((u8 *)"00005");//������ת����
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"TurnLeft")==0)
		{
			action_flag = 6;
			Voice_Play((u8 *)"00004");//������ת����
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		if(strcmp((const char *)u2.buff,"Twist")==0)
		{
			action_flag = 7;
			Voice_Play((u8 *)"00006");//������ת����
			memset(u2.buff,0,sizeof(u2.buff));//�������
		}
		switch(action_flag)
		{
			case 0:
				Arm_Li();//ִ��վ������
			break;
			case 1:
				Arm_Push();//ִ�и��ԳŶ���
			break;
			case 2:
				Arm_Jump();//ִ�����趯��
			break;
			case 3:
				Arm_Hello();//ִ�д��к�����
			break;
			case 4:
				Arm_Forward();//ִ��ǰ������
			break;
			case 5:
				Arm_Right();//ִ����ת����
			break;
			case 6:
				Arm_Left();//ִ����ת����
			break;
			case 7:
				Arm_Twisting();//ִ��Ť������
			break;
		}
		
	}
	
}


