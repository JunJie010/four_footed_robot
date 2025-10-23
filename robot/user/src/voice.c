#include "main.h"


const  char *voice_buff[] = {
	"00001",		//ֹͣ
	"00002",		//ֱ��
	"00003",		//����
	"00004",		//��ת
	"00005",		//��ת
	"00006",		//Ť��
	"00007",		//�䶯
	"00008",		//����
	"00009",		//���к�
	"00010"			//���Գ�
};



/***************************************************
*������    ��voice_init
*��������  ����������5��ʼ������
*��������  ��u32 bound    ������
*��������ֵ����
*��������  ��	UART5_TX --- PC12  --- �����������
						  UART5_RX --- PD2 	 --- ��������ģʽ
****************************************************/
void voice_init(u32 bound)
{
	
	//1����GPIOCʱ�ӡ�GPIODʱ�Ӻ�UART5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	
	//2����ʼ��PC12��PD2  ---  GPIO����
	GPIO_InitTypeDef GPIO_InitStruct={0};//����ṹ�������
	//PC12    UART5_TX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//PC12����Ϊ�����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;//���õ�12�Źܽ�
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//����ٶ�Ϊ50Mhz
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	//PD2   UART5_RX    ��������ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//PD2����Ϊ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//���õ�2�Źܽ�
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	
	//3��UART5��ʼ��
	USART_InitTypeDef USART_InitStruct={0};
	USART_InitStruct.USART_BaudRate = bound;//���ò�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��ʹ��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�������ͷ�����ʹ��
	USART_InitStruct.USART_Parity = USART_Parity_No;//��ʹ����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//ֹͣλΪ1bit
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8bit
	USART_Init(UART5,&USART_InitStruct);
	
	//4��ʹ��UART5   
	USART_Cmd(UART5,ENABLE);

}



/***************************************************
*������    ��uart5_send_byte
*��������  ������5����һ�ֽ����ݺ���
*��������  ��u8 data
*��������ֵ����
*��������  ��	PC12--Uart5_Tx   PD2--Uart5_Rx
1���ȴ����ݿ��Է��ͣ��ȴ����ͱ�־λ��һ��
2�����÷������ݺ�����������
****************************************************/
void uart5_send_byte(u8 data)
{
	//��ѯ��ⷢ�����ݼĴ����Ƿ�Ϊ��
	while(USART_GetFlagStatus(UART5,USART_FLAG_TXE) == RESET);
	USART_SendData(UART5,data); //��������
}

//����
u8 Play[4]={0xAA,0x02,0x00};

u8 Stop[4]={0xAA,0x03,0x00};

u8 Next[4]={0xAA,0x05,0x00};

u8 Vol_Up[4]={0xAA,0x14,0x00};

u8 Vol[4]={0xAA,0x13 ,0x01 ,0x1e};


/***************************************************
*������    ��Voice_SendData
*��������  ����������5�������ݰ�����
*��������  ��u8 *cmd      ��Ҫ����ָ����
						 u8 cmd_len   ����ָ�������ݳ���
*��������ֵ����
*��������  ��	PC12--Uart5_Tx   PD2--Uart5_Rx
****************************************************/
void Voice_SendData(u8 *cmd,u8 cmd_len)
{
	u8 cheak_sum = 0;
	for(u8 i=0;i<cmd_len;i++)
	{
		cheak_sum += *cmd;//����У���
		uart5_send_byte(*cmd++);
		
	}
	//����У���,ȡ�Ͱ�λ
	uart5_send_byte((cheak_sum & 0xff));
}


/***************************************************
*������    ��Voice_Play
*��������  ����������ָ��ִ����Ӧ���ܺ���
*��������  ��u8 *name
*��������ֵ����
*��������  ��
"00001"  --  ֹͣ
"00002"  --  ֱ��
"00003"  --  ����
"00004"  --  ��ת
"00005"  --  ��ת
"00006"  --  Ť��
"00007"  --  �䶯
"00008"  --  ����
"00009"  --  ���к�
"00010"  --  ���Գ�
"00011"  --  ��ʼ������
"00012"  --  ��ʼ���ɹ�
"00013"  --  δ֪
"00014"  --  ��ʼ���ɹ�
"00015"  --  ֻ����̫��
"00016"  --  ��ӭʹ��
****************************************************/
void Voice_Play(u8 *name)
{
	u8 mix_data[40]={0xAA,0x08,0x00,0x02};//ָ���̷�ָ��·������ 0x02���̷�
	u8 temp[10];//�ݴ�·������·����ŵ�temp�����У�temp[10] = "/name*???"
	sprintf((char *)temp,"/%s*???",name);
	//����·������
	mix_data[2] = strlen((const char *)temp)+1;//���ݳ���=�̷�����+·������=1+·������
	//ƴ��·��
	strcpy((char *)(mix_data+4),(const char *)temp);//��·���ӵ�mix_data��
	Voice_SendData(mix_data,strlen((const char *)mix_data));//��ָ�����ݷ��͸��ӻ�
	
}


/***************************************************
*������    ��Set_Song_Volume
*��������  ������������С����
*��������  ��unsigned char volume_num ������С 0---30			 
*��������ֵ����
*��������  ��
****************************************************/
void Set_Song_Volume(unsigned char volume_num)
{
	//������������
	char Play_song_volume[4]={0xAA,0x13,0x01,0x09};
	//����ֵ���и�ֵ
	Play_song_volume[3] = volume_num;
	Voice_SendData((u8*)Play_song_volume,sizeof(Play_song_volume));
}


