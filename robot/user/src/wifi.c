#include "main.h"

//����USART2
/*
�������ܣ�USART2��ʼ��
�βΣ�u32 bps    ������
����ֵ��void
����˵����
USART2_TX --- PA2  --- �����������
USART2_RX --- PA3 --- ��������ģʽ
*/

void Usart2_Init(u32 bps)
{
	//1����GPIOAʱ�Ӻ�USART2ʱ��  RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	//2����ʼ��PA2��PA3  ---  GPIO����
	GPIO_InitTypeDef GPIO_InitStruct={0};//����ṹ�������
	//PA2    USART2_TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//PA2����Ϊ�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//���õ�2�Źܽ�
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�Ϊ50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA3   USART2_RX    ��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PA3����Ϊ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;//���õ�3�Źܽ�
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3��USART2��ʼ��
	USART_InitTypeDef USART_InitStruct={0};
	USART_InitStruct.USART_BaudRate = bps;//���ò�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�������ͷ�����ʹ��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλΪ1bit
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8bit
	USART_Init(USART2,&USART_InitStruct);
	
	//4��ʹ��USART2�����жϺͿ����ж�    XXX_ITConfig();
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//�����ж�
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//�����ж�
	
	//5������NVIC
	NVIC_InitTypeDef NVIC_InitStruct={0};
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;//ѡ���ж�Դ
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�Դͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//ռ�����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//�μ����ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
	//6��ʹ��USART2   
	USART_Cmd(USART2,ENABLE);
}

/*
USART2�жϷ�����
*/
U2_STRUCT u2={0};
void USART2_IRQHandler(void)
{
	/*дUSART2�жϷ�����ʱ����Ҫ�жϽ��������һ���ж�*/
	//�˴��жϽ�������ж�
	if(USART_GetITStatus(USART2,USART_IT_RXNE))//�˱�־λӲ����һ����Ҫ�������
	{
		//�����־λ
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		//�������ݲ���    �����ַ�����ֵ    �����������¼�������ݳ��ȣ�
		/*len�������׼ӵ�ʲôʱ���ֹͣ���ſ�������*/
		u2.buff[u2.len++] = USART_ReceiveData(USART2);
		
		
	}
	//�˴��жϽ�������ж�   ��ʾ���ݽ������
	if(USART_GetITStatus(USART2,USART_IT_IDLE))
	{
		//�����־λ   IDLE���б�־λ������������Ĳο��ֲ�Ĵ�������
		/*�ȶ�USART2_SR�Ĵ������ٶ�USART2_DR*/
		USART2->SR;
		USART2->DR;
		
		//���ݽ������
		u2.buff[u2.len] = '\0';//���ַ������һ�ֽڲ��������
		u2.len = 0;//�������ݳ��ȣ�׼��������һ������
		u2.flag = 1;//�������ݽ������
		printf("%s",u2.buff);
	}	
}



/*********************���ܺ���********************/

/*
�������ܣ�USART2����1�ֽ�����
�βΣ�u8 data   ��Ҫ���͵�1byte����
����ֵ��void
����˵����
1���ȴ����ݿ��Է��ͣ��ȴ����ͱ�־λ��һ��
2�����÷������ݺ�����������
*/

void usart2_send_byte(u8 data)
{
	//1���ȴ����ݿ��Է��ͣ��ȴ����ͱ�־λ��һ��
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE))
	{
		
	}
	//2�����÷������ݺ�����������
	USART_SendData(USART2,data);
}

/*
�������ܣ�USART2�����ַ�������
�βΣ�u8 *data   ��Ҫ���͵��ַ���
����ֵ��void
����˵����"hello world\0"
�ַ����������ַ�����β'\0'
*/

void usart2_send_str(u8 *data)
{
	while(*data != '\0')//��*data��ֵΪ'\0'ʱ����ѭ��
	{
		usart2_send_byte(*data);
		data++;
	}	
}

/*
�������ܣ�Esp8266_SendCmd   ESP8266�����������ж��Ƿ�ɹ�ִ��
�βΣ�
u8 *cmd       ����ATָ��
u8 *respond   ESP8266��Ҫ��Ӧ�Ľ��   "OK"
����ֵ��u8    0���ɹ�     1��ʧ��
*/

u8 Esp8266_SendCmd(u8 *cmd,u8 *respond)
{
	usart2_send_str(cmd);//����ATָ��
	delay_ms(300);
	//�жϴ�ָ��ESP8266�Ƿ�ɹ�ִ��
	while(1)
	{
		//�ж������Ƿ�������
		if(u2.flag == 1)
		{
			u2.flag = 0;
			if(strstr((const char *)u2.buff,(const char *)respond) != NULL)
				return 0;
			else
				return 1;
		}
	}

}



/*
�������ܣ�ESP8266_Init   WIFI��ʼ��
�βΣ�void
����ֵ��u8
*/

u8 ESP8266_Init(void)
{
	
	Voice_Play((u8 *)"00011");//��ʼ������
	
	u8 ret = 0;
	Usart2_Init(115200);//115200   ESP8266Ĭ�ϲ����ʣ�115200
	
	//���˳�͸��
	usart2_send_str((u8 *)"+++");
	delay_ms(300);
	//����㷢��һ��ATָ���������
	usart2_send_str((u8 *)"AT\r\n");
	delay_ms(300);
	//�ۻָ���������
	usart2_send_str((u8 *)"AT+RESTORE\r\n");
	delay_ms(2000);
	//������ESP8266Ϊ�ͻ���ģʽ     ��ģʽ����������·��
	ret = Esp8266_SendCmd((u8 *)"AT+CWMODE=1\r\n",(u8 *)"OK");
	if(ret != 0)
	{
		printf("ESP8266���ÿͻ���ģʽʧ��\r\n");
		return 1;
	}
		printf("ESP8266���ÿͻ���ģʽ�ɹ�\r\n");
	//������ESP8266Ϊ͸��ģʽ
	ret = Esp8266_SendCmd((u8 *)"AT+CIPMODE=1\r\n",(u8 *)"OK");
	if(ret != 0)
	{
		printf("ESP8266����͸��ģʽʧ��\r\n");
		return 2;
	}
	printf("ESP8266����͸��ģʽ�ɹ�\r\n");
	
	printf("ESP8266��ʼ���ɹ�\r\n");
	return 0;
	
}


/*
�������ܣ�ESP8266_ConnectWifi    ������·��
�βΣ�
u8 *ssid     ��дWIFI�˺���
u8 *pwd      ��дWIFI����
����ֵ��u8     0:WIFI���ӳɹ�      1:WIFI����ʧ��
����˵����
������·��ָ�
AT+CWJAP="WIFI�˺���","WIFI����"\r\n
*/

u8 ESP8266_ConnectWifi(u8 *ssid,u8 *pwd)
{
	u8 cnt = 0;
	//���ַ���ƴ��    ��sprintf
	u8 wifi_buff[150] = {0};
	sprintf((char *)wifi_buff,"AT+CWJAP=\"%s\",\"%s\"\r\n",ssid,pwd);
	A:
	usart2_send_str((u8 *)wifi_buff);//����ƴ�Ӻõ�ָ��
	while(1)
	{
		while(!u2.flag);//�ȴ����ݽ������
		u2.flag = 0;
		
		if(strstr((const char *)u2.buff,"OK") != NULL)//WIFI���ӳɹ�
		{
			printf("WIFI���ӳɹ�!!!!\r\n");
			return 0;
		}
		if(strstr((const char *)u2.buff,"FAIL") != NULL)//WIFI����ʧ��
		{
			printf("WIFI����ʧ�ܣ����ڵ�%d����������....\r\n",++cnt);
			if(cnt == 3)
			{
				printf("WIFI����ʧ��!!!!!\r\n");
				return 1;
			}
			goto A;
		}	
	}	
}



/*
�������ܣ�ESP8266_ConnectServer    ���ӷ�����
�βΣ�
u8 *ip        ��дIP��ַ
u8 *port      ��д�˿ں�
����ֵ��u8     0:WIFI���ӳɹ�      1:WIFI����ʧ��
����˵����
���ӷ�����ָ�
AT+CIPSTART="TCP","192.168.31.207",8080\r\n

���Զˣ������ֻ��ȵ�
��IP���ٿ���̨ win + R ����cmd���ڿ���̨������ipconfig���س�
			������λ������в��ң�
*/

//u8 ESP8266_ConnectServer(u8 *ip,u8 *port)
//{
//	u8 cnt = 0;
//	//���ַ���ƴ��    ��sprintf
//	u8 wifi_buff[200] = {0};
//	sprintf((char *)wifi_buff,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",ip,port);
//	A:
//	usart2_send_str((u8 *)wifi_buff);//����ƴ�Ӻõ�ָ��
//	while(1)
//	{
//		while(!u2.flag);//�ȴ����ݽ������
//		u2.flag = 0;
//		
//		if(strstr((const char *)u2.buff,"OK") != NULL)//���ӷ������ɹ�
//		{
//			printf("���ӷ������ɹ�!!!!\r\n");
//			usart2_send_str((u8 *)"AT+CIPSEND\r\n");//����ESP8266Ϊ��������ģʽ
//			return 0;
//		}
//		if(strstr((const char *)u2.buff,"ERROR") != NULL)//���ӷ�����ʧ��
//		{
//			printf("���ӷ�����ʧ�ܣ����ڵ�%d����������....\r\n",++cnt);
//			if(cnt == 3)
//			{
//				printf("���ӷ�����ʧ��!!!!!\r\n");
//				return 1;
//			}
//			goto A;
//		}	
//	}	
//}


u8 Esp8266_ConnectServer(u8 *ip,u8 *port)
{
	
	u8 cat[200]={0};
	u8 sever_cnt = 0;
	strcat((char *)cat,"AT+CIPSTART=\"TCP\",\"");
	strcat((char *)cat,(const char *)ip);
	strcat((char *)cat,"\",");
	strcat((char *)cat,(const char *)port);
	strcat((char *)cat,"\r\n");
	B:
	usart2_send_str(cat);//����ƴ����ɵ�ָ��
	
	while(1)
	{
		while(!u2.flag);//�ȴ����ݽ������
		u2.flag = 0;
		
		if(strstr((const char *)u2.buff,"OK")!=NULL)
		{
			printf("���ӷ������ɹ�\r\n");
			usart2_send_str((u8 *)"AT+CIPSEND\r\n");//���뷢��ģʽ����    >
			return 0;
		}
		if(strstr((const char *)u2.buff,"ERROR")!=NULL)
		{
			if(sever_cnt == 3)
			{
				printf("���ӷ�����ʧ��\r\n");
				return 1;
			}
			printf("���ӷ�����ʧ�ܣ����ڳ��Ե�%d������\r\n",++sever_cnt);
			goto B;
		}
		
	}		
}










